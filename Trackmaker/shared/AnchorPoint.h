//
//  Point.h
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#ifndef __Trackmaker__Point__
#define __Trackmaker__Point__

#include "Vec4.h"

class CoordinateSystem;

class AnchorPoint {
	float x, y, z;
	float direction;
	float inclination;
	float bank;
	float scaleWidth;
	float scaleHeight;
	float nextControlPointDistanceFactor;
	float previousControlPointDistanceFactor;
	
public:
	void setPosition(float aX, float aY, float aZ) {
		x = aX;
		y = aY;
		z = aZ;
	}
	void getPosition(float &aX, float &aY, float &aZ) const {
		aX = x;
		aY = y;
		aZ = z;
	}
	
	void setDirection(float angleInRadian) { direction = angleInRadian; }
	float getDirection() const { return direction; }
	
	void setInclination(float angleInRadian) { inclination = angleInRadian; }
	float getInclination() const { return inclination; }
	
	void setBank(float angleInRadian) { bank = angleInRadian; }
	float getBank() const { return bank; }
	
	void setScaleWidth(float scale) { scaleWidth = scale; }
	float getScaleWidth() const { return scaleWidth; }
	
	void setScaleHeight(float scale) { scaleHeight = scale; };
	float getScaleHeight() const { return scaleHeight; }
	
	void setNextControlPointDistanceFactor(float factor) { nextControlPointDistanceFactor = factor; }
	float getNextControlPointDistanceFactor() const { return nextControlPointDistanceFactor; }
	
	void setPreviousControlPointDistanceFactor(float factor) { previousControlPointDistanceFactor = factor; }
	float getPreviousControlPointDistanceFactor() const { return previousControlPointDistanceFactor; }
	
	matrix getMatrix() const;
};

#endif /* defined(__Trackmaker__Point__) */
