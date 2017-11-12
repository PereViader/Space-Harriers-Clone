#pragma once

#include "Object.h"
#include "Component.h"

#include <string>
#include <vector>

using namespace std;

class GameObject :
	public Object
{
public:
	GameObject(bool enabled);
	~GameObject();

	Component * AddComponent(string type);
	Component * GetComponent(string type);
	const vector<Component*>& GetComponents();

private:
	vector<Component*> _components;
};
