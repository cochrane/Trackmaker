//
//  Template.cpp
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#include "Template.h"

#include <fstream>

Template::Template(const char *path)
{
	std::ifstream stream(path);
	
	automaticU = false;
	automaticV = false;
	
	while (stream)
	{
		std::string tag;
		stream >> tag;
		if (tag == "maximumLength")
			stream >> maximumLength;
		else if (tag == "automaticUFactor")
		{
			automaticU = true;
			stream >> automaticUFactor;
		}
		else if (tag == "automaticVFactor")
		{
			automaticV = true;
			stream >> automaticVFactor;
		}
		else if (tag == "v")
		{
			Vertex v;
			stream >> v.position[0];
			stream >> v.position[1];
			stream >> v.position[2];
			stream >> v.normal[0];
			stream >> v.normal[1];
			stream >> v.normal[2];
			stream >> v.texCoord[0];
			stream >> v.texCoord[1];
			vertices.push_back(v);
		}
		else if (tag == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				int index;
				stream >> index;
				indices.push_back(index);
			}
		}
	}
}
