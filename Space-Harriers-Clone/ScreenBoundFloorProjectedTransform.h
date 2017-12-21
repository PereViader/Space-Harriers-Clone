#ifndef _ScreenBoundFloorProjectedTransform_H_
#define _ScreenBoundFloorProjectedTransform_H_

#include "FloorRelatedTransform.h"
#include "Vector3.h"

class ScreenBoundFloorProjectedTransform :
	public FloorRelatedTransform
{
public:
	ScreenBoundFloorProjectedTransform();
	virtual ~ScreenBoundFloorProjectedTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual float GetDepth() const override;
	virtual float GetRenderingScale() const override;
	virtual void Move(const Vector3 & movement) override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual ScreenBoundFloorProjectedTransform * Clone() const override;

private:
	Vector3 position;
};

#endif // !_ScreenBoundFloorProjectedTransform_H_