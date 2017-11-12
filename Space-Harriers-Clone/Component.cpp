#include "Component.h"
#include <string>

using namespace std;


Component::Component(bool enabled, string name) :
	_name(name),
	Object(enabled)
{
}


Component::~Component()
{
}