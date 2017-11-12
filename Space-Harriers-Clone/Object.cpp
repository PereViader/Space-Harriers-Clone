#include "Object.h"


Object::Object(bool enabled) :
	_enabled(enabled),
	_isDead(false)
{
}

Object::~Object()
{
}

bool Object::IsEnabled()
{
	return _enabled;
}
