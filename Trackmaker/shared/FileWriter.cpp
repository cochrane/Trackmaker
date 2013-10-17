//
//  FileWriter.cpp
//  Trackmaker
//
//  Created by Torsten Kammer on 17.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#include "FileWriter.h"

#include <fstream>

#include "AnchorPoint.h"
#include "Track.h"

void FileWriter::writeObj(std::ostream &stream)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	
	track.fillBuffers(vertices, indices);
	
	for (const Vertex &vertex : vertices)
	{
		stream << "v " << vertex.position[0] << " " << vertex.position[1] << " " << vertex.position[2] << std::endl;
		stream << "vn " << vertex.normal[0] << " " << vertex.normal[1] << " " << vertex.normal[2] << std::endl;
		stream << "vt " << vertex.texCoord[0] << " " << vertex.texCoord[1] << std::endl;
	}
	
	for (int index = 0; index < indices.size(); index += 3)
	{
		stream << "f ";
		for (int corner = 0; corner < 3; corner++)
			stream << (indices[index+corner]+1) << "/" << (indices[index+corner]+1) << "/" << (indices[index+corner]+1) << " ";
		stream << std::endl;
	}
}

void FileWriter::writeCollada(std::ostream &stream)
{
	// TODO: Fill this
}

void FileWriter::write(FileType type, const char *path)
{
	std::ofstream stream(path);
	switch (type) {
		case OBJ:
			writeObj(stream);
			break;
		case COLLADA:
			writeCollada(stream);
			break;
	}
}