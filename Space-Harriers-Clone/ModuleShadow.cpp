#include "ModuleShadow.h"

#include <assert.h>

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "Pivot2D.h"
#include "Transform.h"


const float ModuleShadow::SHADOW_SCALE_FACTOR = 5.0f;

ModuleShadow::ModuleShadow()
{
}


ModuleShadow::~ModuleShadow()
{
}

bool ModuleShadow::Start()
{
	shadow = App->textures->Load("rtype/Character_full.png");
	assert(shadow.GetTexture() != nullptr);

	Animation animation;
	animation.frames.push_back({ 197,76,43,20 });
	shadow.UpdateTexture(animation);
	return true;
}

void ModuleShadow::DrawShadow(const Transform & t)
{
	Vector3 screen = t.GetFloorPositionAndDepth();
	float scale = SHADOW_SCALE_FACTOR * t.GetRenderingScale();
	App->renderer->BlitWithPivotScaledZBuffer(shadow, scale, Pivot2D::MIDDLE_CENTER, screen );
}

bool ModuleShadow::CleanUp()
{
	App->textures->Unload(shadow);
	return true;
}