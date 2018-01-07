#ifndef _BACKGROUND_LAYER_H
#define _BACKGROUND_LAYER_H

#include "Texture.h"

class BackgroundLayer
{
public:
	BackgroundLayer(const Texture& texture, float speed, float scalingFactor);
	~BackgroundLayer();

	void Update();
	void Render();

private:
	Texture graphics;
	float textureOffset;

	float textureHeight;
	float textureWidth;

	float scalingFactor;

	float speed;
};

#endif // !_BACKGROUND_LAYER_H