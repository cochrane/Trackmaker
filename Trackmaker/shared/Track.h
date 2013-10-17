//
//  Track.h
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#ifndef __Trackmaker__Track__
#define __Trackmaker__Track__

#include <vector>

#include "Template.h"
#include "Vec4.h"

class Template;
class AnchorPoint;

class Track {
	Template &trackTemplate;
	std::vector<AnchorPoint> anchorPoints;
	
	struct InterpolationResult {
		matrix position;
		float lengthFromStart;
	};
	
	float recursiveCalculateUnbankedMatrices(const float4 *controlPoints, std::vector<InterpolationResult> &result);
	std::vector<InterpolationResult> *calculateMatrices();
	
public:
	Track(Template &trackTemplate) : trackTemplate(trackTemplate) {}
	
	const Template &getTemplate() const { return trackTemplate; }
	const std::vector<AnchorPoint> &getAnchorPoints() const { return anchorPoints; }
	AnchorPoint &createAnchorPointAtEnd();
	
	void fillBuffers(std::vector<Vertex> &vertices, std::vector<unsigned> &indices);
};

#endif /* defined(__Trackmaker__Track__) */
