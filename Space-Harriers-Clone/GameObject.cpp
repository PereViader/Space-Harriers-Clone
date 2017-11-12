#include "GameObject.h"

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

GameObject::GameObject(bool enabled) :
	Object(enabled)
{
}


GameObject::~GameObject()
{
	for (Component * component : _components)
		delete component;
}

Component * GameObject::AddComponent(string type)
{
	Component * component = nullptr; // TODO: make component fabric
	_components.push_back(component);
	return component;
}

Component * GameObject::GetComponent(string type)
{
	for (Component * component : _components)
		if (component->_name == type)
			return component;
	return nullptr;
}

const vector<Component*>& GameObject::GetComponents()
{
	return _components;
}
