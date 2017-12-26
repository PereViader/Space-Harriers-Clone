#pragma once

#include "IRenderable.h"
#include "Texture.h"
#include "Vector3.h"
#include "Size2D.h"

struct SDL_Texture;

class UI_HealthPoints :
	public IRenderable
{
public:
	UI_HealthPoints();
	~UI_HealthPoints();

	virtual void Render() override;

private:
	static const Size2D HEALTH_SIZE;

	int previousHealthPoints;

	Texture baseGraphics;
	//Texture healthPointsCachedGraphic;

private:
	//Texture BuildTextureForHealthPoints(int healthPoints) const;
	//void UnloadCachedTexture(Texture& texture);
	static const Vector3 RENDERING_POSITION;

};

