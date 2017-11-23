#pragma once

#include "Point.h"
#include "Animation.h"

struct Collider;

class Enemy
{
public:
	virtual ~Enemy() {};

	virtual Enemy* Clone() const = 0;
	virtual void Update() = 0;

public:
	bool to_delete;

protected:
	
};

