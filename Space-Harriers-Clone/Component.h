#pragma once

#include "Object.h"
#include <string>

using namespace std;

class Component :
	public Object
{
public:
	Component(bool enabled, string name);
	virtual ~Component();

	const string _name;
private:
};

