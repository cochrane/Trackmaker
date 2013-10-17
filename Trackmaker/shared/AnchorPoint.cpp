//
//  Point.cpp
//  Trackmaker
//
//  Created by Torsten Kammer on 14.10.13.
//  Copyright (c) 2013 Torsten Kammer. All rights reserved.
//

#include "AnchorPoint.h"

matrix AnchorPoint::getMatrix() const
{
	matrix result = matrix::rotation(float4(0, 1, 0, 0), getDirection());
	result = matrix::rotation(result * float4(0, 0, 1, 0), getInclination()) * result;
	result = matrix::rotation(result * float4(1, 0, 0, 0), getBank()) * result;
	result.w = float4(x, y, z);
	return result;
}
