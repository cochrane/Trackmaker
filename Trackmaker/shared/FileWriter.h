//
//  FileWriter.h
//  Trackmaker
//
//  Created by Torsten Kammer on 17.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#ifndef __Trackmaker__FileWriter__
#define __Trackmaker__FileWriter__

#include <ostream>

class Track;

enum FileType {
	OBJ,
	COLLADA
};

class FileWriter {
	Track &track;
	
	void writeObj(std::ostream &stream);
	void writeCollada(std::ostream &stream);
	
public:
	FileWriter(Track &aTrack) : track(aTrack) {}
	
	void write(FileType type, const char *path);
};

#endif /* defined(__Trackmaker__FileWriter__) */
