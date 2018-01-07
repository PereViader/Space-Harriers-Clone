#ifndef _FLOOR_RELATED_TRANSFORM_H__
#define _FLOOR_RELATED_TRANSFORM_H__

#include "Transform.h"
class FloorRelatedTransform :
	public Transform
{
public:
	virtual Vector3 GetFloorPositionAndDepth() const = 0;
	virtual FloorRelatedTransform* Clone() const override = 0;
};

#endif // !_FLOOR_RELATED_TRANSFORM_H__