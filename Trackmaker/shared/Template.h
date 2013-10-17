//
//  Template.h
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#ifndef __Trackmaker__Template__
#define __Trackmaker__Template__

#include <vector>

struct Vertex {
	float position[3];
	float normal[3];
	float texCoord[2];
};

class Template {
	float maximumLength;
	
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	
	bool automaticU;
	bool automaticV;
	float automaticUFactor;
	float automaticVFactor;

public:
	Template(const char *templatePath);
	
	float getMaximumLength() const { return maximumLength; }
	
	const std::vector<Vertex> &getVertices() const { return vertices; }
	const std::vector<int> &getIndices() const { return indices; }
	
	bool usesAutomaticU() const { return automaticU; }
	float getAutomaticUFactor() const { return automaticUFactor; }
	
	bool usesAutomaticV() const { return automaticV; }
	float getAutomaticVFactor() const { return automaticVFactor; }
};

#endif /* defined(__Trackmaker__Template__) */
