#ifndef _SCREEN_BOUND_FLOOR_PROJECTED_TRANSFORM_H__
#define _SCREEN_BOUND_FLOOR_PROJECTED_TRANSFORM_H__

#include "FloorRelatedTransform.h"
#include "Vector3.h"

class ScreenBoundFloorProjectedTransform :
	public FloorRelatedTransform
{
public:
	ScreenBoundFloorProjectedTransform();
	ScreenBoundFloorProjectedTransform(const Vector3&);
	virtual ~ScreenBoundFloorProjectedTransform();

	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual float GetDepth() const override;
	virtual float GetRenderingScale() const override;
	virtual void Move(const Vector3 & movement) override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual ScreenBoundFloorProjectedTransform * Clone() const override;

	void SetPosition(const Vector3&);

private:
	Vector3 position;
};

#endif // !_SCREEN_BOUND_FLOOR_PROJECTED_TRANSFORM_H__