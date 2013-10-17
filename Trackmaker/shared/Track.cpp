//
//  Track.cpp
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#include "Track.h"

#include <iostream>

#include "AnchorPoint.h"

float Track::recursiveCalculateUnbankedMatrices(const float4 *controlPoints, std::vector<InterpolationResult> &result) {
	const float length = (controlPoints[0] - controlPoints[3]).length();
	if (length < trackTemplate.getMaximumLength())
	{
		// Calculate the end matrix for the section. The start matrix will have been calculated beforehand
		InterpolationResult newPoint;
		// Forward: last control point to end point is tangent
		newPoint.position.x = (controlPoints[3] - controlPoints[2]).normalized();
		// Up: For sufficiently small distances, using the previous up vector is a good first approximation. Since there is no banking, it should lie in the right plane. I hope.
		newPoint.position.y = float4(0, 1, 0, 0);
		// Right: Cross product
		newPoint.position.z = newPoint.position.x.cross(newPoint.position.y).normalized();
		// Get actual up value
		newPoint.position.y = newPoint.position.z.cross(newPoint.position.x).normalized();
		
		// Apply position
		newPoint.position.w = controlPoints[3];
		
		if (std::isnan(newPoint.position.w.x) || std::isnan(newPoint.position.x.x)) {
			std::cout << "Nan!";
		}
		
		newPoint.lengthFromStart = length;
		result.push_back(newPoint);
		
		return length;
	}
	else
	{
		// Split the given bezier path using the method of DeCateljau (spelling?)
		const float4 firstMidpoints[3] = {
			0.5f * (controlPoints[0] + controlPoints[1]),
			0.5f * (controlPoints[1] + controlPoints[2]),
			0.5f * (controlPoints[2] + controlPoints[3]),
		};
		
		const float4 secondMidpoints[2] = {
			0.5f * (firstMidpoints[0] + firstMidpoints[1]),
			0.5f * (firstMidpoints[1] + firstMidpoints[2]),
		};
		
		const float4 thirdMidpoint = 0.5f * (secondMidpoints[0] + secondMidpoints[1]);
		
		const float4 firstPartialCurve[4] = {
			controlPoints[0],
			firstMidpoints[0],
			secondMidpoints[0],
			thirdMidpoint
		};
		float part1Length = recursiveCalculateUnbankedMatrices(firstPartialCurve, result);
		size_t part1PointCount = result.size();
		
		const float4 secondPartialCurve[4] = {
			thirdMidpoint,
			secondMidpoints[1],
			firstMidpoints[2],
			controlPoints[3]
		};
		float part2Length = recursiveCalculateUnbankedMatrices(secondPartialCurve, result);
		// Update lengths
		for (size_t i = part1PointCount; i < result.size(); i++) {
			result[i].lengthFromStart += part1Length;
		}
		
		return part1Length + part2Length;
	}
}

std::vector<Track::InterpolationResult> *Track::calculateMatrices()
{
	std::vector<InterpolationResult> *matrices = new std::vector<InterpolationResult>();
	
	InterpolationResult start;
	start.position = anchorPoints.begin()->getMatrix();
	start.lengthFromStart = 0;
	matrices->push_back(start);
	
	float totalTrackLength = 0;
	
	for (size_t i = 0; i < anchorPoints.size() - 1; i++)
	{
		const size_t numberOfMatricesSoFar = matrices->size();
		
		const AnchorPoint &startPoint = anchorPoints[i];
		const AnchorPoint &endPoint = anchorPoints[i+1];
		
		// Calculate the control points for this bezier patch
		const matrix start = startPoint.getMatrix();
		const matrix end = endPoint.getMatrix();
		const float length = (start.w - end.w).length();
		
		const float4 controlPoint1 = start.w + start.x * length * anchorPoints[i].getNextControlPointDistanceFactor();
		const float4 controlPoint2 = end.w - end.x * length * anchorPoints[i+1].getPreviousControlPointDistanceFactor();
		
		// Get the actual matrices
		const float curveLength = recursiveCalculateUnbankedMatrices((float4[]) { start.w, controlPoint1, controlPoint2, end.w}, *matrices);
		
		// Apply the bank angle to them.
		for (size_t matrixIndex = numberOfMatricesSoFar; matrixIndex < matrices->size(); matrixIndex++) {
			const float interpolationFactor = ((*matrices)[matrixIndex].lengthFromStart - totalTrackLength) / curveLength;
			
			// First scale
			matrix &position = (*matrices)[matrixIndex].position;
			matrix scaleMatrix;
			scaleMatrix[1][1] = interpolationFactor * startPoint.getScaleHeight() + (1.0 - interpolationFactor) * endPoint.getScaleHeight();
			scaleMatrix[2][2] = interpolationFactor * startPoint.getScaleWidth() + (1.0 - interpolationFactor) * endPoint.getScaleWidth();
			
			position = scaleMatrix * position;
			
			// Add bank as final transform
			const matrix bankMatrix = matrix::rotation(position.x, interpolationFactor * startPoint.getBank() + (1.0 - interpolationFactor) * endPoint.getBank());
			position = bankMatrix * position;
			
			// Set correct length for this interpolation result
			(*matrices)[matrixIndex].lengthFromStart += totalTrackLength;
		}
		
		// Update track length
		totalTrackLength += curveLength;
	}
	
	return matrices;
}

AnchorPoint &Track::createAnchorPointAtEnd()
{
	AnchorPoint newPoint;
	if (anchorPoints.size() > 0)
	{
		float x, y, z;
		anchorPoints.back().getPosition(x, y, z);
		newPoint.setPosition(x, y, z);
		newPoint.setInclination(anchorPoints.back().getInclination());
		newPoint.setBank(anchorPoints.back().getBank());
		newPoint.setDirection(anchorPoints.back().getDirection());
		newPoint.setNextControlPointDistanceFactor(anchorPoints.back().getNextControlPointDistanceFactor());
		newPoint.setPreviousControlPointDistanceFactor(anchorPoints.back().getPreviousControlPointDistanceFactor());
		newPoint.setScaleHeight(anchorPoints.back().getScaleHeight());
		newPoint.setScaleWidth(anchorPoints.back().getScaleWidth());
	}
	else
	{
		newPoint.setPosition(0, 0, 0);
		newPoint.setBank(0);
		newPoint.setInclination(0);
		newPoint.setDirection(0);
		newPoint.setNextControlPointDistanceFactor(0.5);
		newPoint.setPreviousControlPointDistanceFactor(0.5);
		newPoint.setScaleHeight(1);
		newPoint.setScaleWidth(1);
	}
	anchorPoints.push_back(newPoint);
	return anchorPoints.back();
}

void Track::fillBuffers(std::vector<Vertex> &vertices, std::vector<unsigned> &indices)
{
	const std::vector<InterpolationResult> *interpolationResults = calculateMatrices();
	
	const std::vector<Vertex> &templateVertices = trackTemplate.getVertices();
	const std::vector<int> &templateIndices = trackTemplate.getIndices();
	const size_t totalNumberOfVertices = templateVertices.size() * interpolationResults->size();
	
	for (const InterpolationResult &result : *interpolationResults)
	{
		const size_t numberOfElementsSoFar = vertices.size();
		
		// Transform the matrices by the vertex and copy them here (applying automatic U or V based on length if that is set)
		for (const Vertex &vertex : templateVertices)
		{
			Vertex transformed;
			float4 transformedPosition = result.position * float4(vertex.position[0], vertex.position[1], vertex.position[2], 1.0);
			memcpy(transformed.position, &transformedPosition, sizeof(transformed.position));
			float4 transformedNormal = result.position * float4(vertex.normal[0], vertex.normal[1], vertex.normal[2], 1.0);
			memcpy(transformed.normal, &transformedNormal, sizeof(transformed.position));
			
			if (trackTemplate.usesAutomaticU())
				transformed.texCoord[0] = trackTemplate.getAutomaticUFactor() * result.lengthFromStart;
			else
				transformed.texCoord[0] = vertex.texCoord[0];
			
			if (trackTemplate.usesAutomaticV())
				transformed.texCoord[1] = trackTemplate.getAutomaticVFactor() * result.lengthFromStart;
			else
				transformed.texCoord[1] = vertex.texCoord[1];
			
			vertices.push_back(transformed);
		}
		
		// Add faces, ignoring those that fall outside the valid range.
		for (int face = 0; face < templateIndices.size(); face += 3) {
			int faceIndices[3] = {
				static_cast<int>(templateIndices[face + 0] + numberOfElementsSoFar),
				static_cast<int>(templateIndices[face + 1] + numberOfElementsSoFar),
				static_cast<int>(templateIndices[face + 2] + numberOfElementsSoFar)
			};
			
			if ((faceIndices[0] < 0 || faceIndices[0] >= totalNumberOfVertices) ||
				(faceIndices[1] < 0 || faceIndices[1] >= totalNumberOfVertices) ||
				(faceIndices[2] < 0 || faceIndices[2] >= totalNumberOfVertices))
				continue;
			
			indices.push_back(faceIndices[0]);
			indices.push_back(faceIndices[1]);
			indices.push_back(faceIndices[2]);
		}
	}
	
	delete interpolationResults;
}
