#include "ModuleBackground.h"

#include <string>
#include <assert.h>
#include <fstream>

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleFloor.h"
#include "ModulePlayer.h"

#include "Globals.h"
#include "Pivot2D.h"

#include "BackgroundLayer.h"

#include "json.hpp"
#include "json_serializer.h"



ModuleBackground::ModuleBackground(bool enabled) : Module(enabled)
{
}

ModuleBackground::~ModuleBackground()
{
}

bool ModuleBackground::Start()
{
	ifstream file("data/backgrounds.json");
	json backgrounds;
	file >> backgrounds;

	for (auto layer : backgrounds["layers"]) {
		Texture texture = App->textures->Load(layer["texturePath"]);
		float speed = layer["speed"];
		float scalingFactor = layer["scalingFactor"];
		backgroundLayers.push_back(new BackgroundLayer(texture, speed, scalingFactor));
	}

	return true;
}

update_status ModuleBackground::Update()
{
	for (BackgroundLayer *backgroundLayer : backgroundLayers) {
		backgroundLayer->Update();
	}

	for (BackgroundLayer *backgroundLayer : backgroundLayers) {
		backgroundLayer->Render();
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleBackground::CleanUp()
{
	for (BackgroundLayer *backgroundLayer : backgroundLayers) {
		delete backgroundLayer;
	}
	backgroundLayers.clear();
	return true;
}