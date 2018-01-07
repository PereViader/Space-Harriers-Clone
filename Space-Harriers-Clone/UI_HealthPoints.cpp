#include "UI_HealthPoints.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Pivot2D.h"

#include "SDL\include\SDL.h"
#include "RectUtil.h"

const Vector3 UI_HealthPoints::RENDERING_POSITION(20,SCREEN_HEIGHT - 15,-1);

const Size2D UI_HealthPoints::HEALTH_SIZE(60, 90);

UI_HealthPoints::UI_HealthPoints() :
	previousHealthPoints(App->player->GetPlayer().GetHealthPoints()),
	baseGraphics(App->textures->Load("data/sprites/GUI.png"))
{
	baseGraphics.UpdateTexture({ 20,30,10,17 });
}

UI_HealthPoints::~UI_HealthPoints()
{
}

void UI_HealthPoints::Render() {
	Vector3 position = RENDERING_POSITION;
	for (int i = 0; i < App->player->GetPlayer().GetHealthPoints(); i++)
	{
		App->renderer->BlitWithPivotScaledZBuffer(baseGraphics, 4, Pivot2D::BOTTOM_LEFT, position);
		position.x += HEALTH_SIZE.width;
	}
}