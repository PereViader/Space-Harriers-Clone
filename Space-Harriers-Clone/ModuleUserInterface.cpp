#include "ModuleUserInterface.h"

#include "ModuleRender.h"

#include "UI_HealthPoints.h"

ModuleUserInterface::ModuleUserInterface(bool enabled) :
	Module(enabled)
{
}

ModuleUserInterface::~ModuleUserInterface()
{
	for (IRenderable* uiElement : uiElements)
		delete uiElement;
	uiElements.clear();
}

bool ModuleUserInterface::Start()
{
	uiElements.push_back(new UI_HealthPoints());
	return true;
}

update_status ModuleUserInterface::Update()
{
	Render();
	return update_status::UPDATE_CONTINUE;
}

void ModuleUserInterface::Render()
{
	for (IRenderable* uiElement : uiElements) {
		uiElement->Render();
	}
}
