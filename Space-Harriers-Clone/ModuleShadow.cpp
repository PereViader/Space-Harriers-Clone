#include "ModuleShadow.h"

#include <assert.h>

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "Pivot2D.h"
#include "Transform.h"


ModuleShadow::ModuleShadow()
{
}


ModuleShadow::~ModuleShadow()
{
}

bool ModuleShadow::Start()
{
	shadow = App->textures->Load("rtype/Character_full.png");
	assert(shadow != nullptr);
	shadowRect = { 197,76,256,225 };
	return true;
}

void ModuleShadow::DrawShadow(const Transform & t)
{
	Vector3 screen = t.GetFloorPositionAndDepth();
	LOG("%f, %f, %f", screen.x, screen.y, screen.z);

	App->renderer->BlitWithPivotScaledZBuffer(shadow, &shadowRect, 1, Pivot2D::MIDDLE_CENTER, screen );
}

bool ModuleShadow::CleanUp()
{
	App->textures->Unload(shadow);
	return true;
}