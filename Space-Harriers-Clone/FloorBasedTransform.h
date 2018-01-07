#ifndef _FLOOR_BASED_TRANSFORM_H__
#define _FLOOR_BASED_TRANSFORM_H__

#include "FloorRelatedTransform.h"

#include "Vector3.h"

class FloorBasedTransform :
	public FloorRelatedTransform
{
public:
	FloorBasedTransform();
	FloorBasedTransform(const Vector3&);
	virtual ~FloorBasedTransform();

	virtual FloorBasedTransform* Clone() const override;
	virtual Vector3 GetScreenPositionAndDepth() const override;
	virtual void Move(const Vector3 & movement) override;
	virtual Vector3 GetFloorPositionAndDepth() const override;
	virtual float GetDepth() const override;
	virtual float GetRenderingScale() const override;

	void SetPosition(const Vector3&);
	const Vector3& GetPosition() const { return position; }

private:
	Vector3 position;
};

#endif // !_FLOOR_BASED_TRANSFORM_H__