#include "BackgroundLayer.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTime.h"
#include "ModuleRender.h"
#include "ModuleFloor.h"
#include "ModuleTextures.h"

#include "Vector2.h"
#include "Pivot2D.h"

BackgroundLayer::BackgroundLayer(const Texture& texture, float speed, float scalingFactor) :
	graphics(texture),
	speed(speed),
	scalingFactor(scalingFactor),
	textureOffset(0),
	textureWidth(static_cast<float>(texture.GetSection().w)),
	textureHeight(static_cast<float>(texture.GetSection().h))
{
}


BackgroundLayer::~BackgroundLayer()
{
	App->textures->Unload(graphics);
}

void BackgroundLayer::Update()
{
	textureOffset = fmodf(textureOffset + App->player->GetPlayer().GetNormalizedPosition().x * speed, textureWidth);
	if (textureOffset < 0)
		textureOffset = textureWidth - textureOffset - 1;
}

void BackgroundLayer::Render()
{
	Vector2 position;
	position.y = App->floor->GetHorizonRenderHeight();
	if (SCREEN_WIDTH >(textureWidth - textureOffset)*scalingFactor) {
		position.x = (textureWidth - textureOffset)*scalingFactor;

		App->renderer->BlitWithPivotScaled(graphics, scalingFactor, Pivot2D::BOTTOM_LEFT, position);
		App->renderer->BlitWithPivotScaled(graphics, scalingFactor, Pivot2D::BOTTOM_RIGHT, position);
	}
	else {
		Pivot2D pivot(textureOffset / textureWidth, 1);

		App->renderer->BlitWithPivotScaled(graphics, scalingFactor, pivot, position);
	}
}
