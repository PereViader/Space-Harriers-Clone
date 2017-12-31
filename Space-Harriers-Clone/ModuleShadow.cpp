#include "ModuleShadow.h"

#include <assert.h>

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "Pivot2D.h"
#include "FloorRelatedTransform.h"


const float ModuleShadow::SHADOW_SCALE_FACTOR = 5.0f;

ModuleShadow::ModuleShadow()
{
}


ModuleShadow::~ModuleShadow()
{
}

bool ModuleShadow::Start()
{
	shadow = App->textures->Load("data/sprites/Character_full.png");
	assert(shadow.GetTexture() != nullptr);
	shadow.UpdateTexture({ 197,76,43,20 });
	return true;
}

void ModuleShadow::DrawShadow(const FloorRelatedTransform & t)
{
	Vector3 screen = t.GetFloorPositionAndDepth();
	screen.z += 0.001;
	float scale = SHADOW_SCALE_FACTOR * t.GetRenderingScale();
	App->renderer->BlitWithPivotScaledZBuffer(shadow, scale, Pivot2D::MIDDLE_CENTER, screen );
}

bool ModuleShadow::CleanUp()
{
	App->textures->Unload(shadow);
	return true;
}