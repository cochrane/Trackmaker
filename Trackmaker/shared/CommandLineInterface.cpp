//
//  CommandLineInterface.cpp
//  Trackmaker
//
//  Created by Torsten Kammer on 17.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#include "CommandLineInterface.h"

#include <fstream>

#include "AnchorPoint.h"
#include "FileWriter.h"
#include "Track.h"
#include "Template.h"

int main(int argc, const char **argv)
{
	if (argc != 4)
	{
		printf("Usage: <template path> <track path> <output path> (currently only obj\n");
		printf("Format for track path: For each point \"p\" then position (three floats), direction (float, degrees), inclination (float, degrees), bank (float, degrees), scale width (float, 1 is normal), scale height (float, 1 is normal)\n");
		exit(-1);
	}
	
	Template trackTemplate(argv[1]);
	Track track(trackTemplate);
	
	std::ifstream stream(argv[2]);
	while (stream.good() && !stream.eof())
	{
		std::string tag;
		stream >> tag;
		
		if (tag == "p")
		{
			AnchorPoint &point = track.createAnchorPointAtEnd();
			
			float x, y, z;
			stream >> x;
			stream >> y;
			stream >> z;
			point.setPosition(x, y, z);
			
			float direction, inclination, bank;
			stream >> direction;
			stream >> inclination;
			stream >> bank;
			point.setDirection(direction * M_PI / 180);
			point.setInclination(inclination * M_PI / 180);
			point.setBank(bank * M_PI / 180);
			
			float scaleWidth, scaleHeight;
			stream >> scaleWidth;
			stream >> scaleHeight;
			point.setScaleWidth(scaleWidth);
			point.setScaleHeight(scaleHeight);
		}
	}
	
	FileWriter writer(track);
	writer.write(FileType::OBJ, argv[3]);
	
	return 0;
}