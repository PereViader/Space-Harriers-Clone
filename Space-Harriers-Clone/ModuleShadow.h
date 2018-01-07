#ifndef _MODULE_SHADOW_H_
#define _MODULE_SHADOW_H_

#include "Module.h"
#include "Texture.h"

class FloorRelatedTransform;

class ModuleShadow :
	public Module
{
public:
	ModuleShadow();
	~ModuleShadow();
	bool Start() override;
	void DrawShadow(const FloorRelatedTransform&);

	bool CleanUp() override;
private:

	static const float SHADOW_SCALE_FACTOR;
	Texture shadow;
	SDL_Rect shadowRect;
};

#endif // !_MODULE_SHADOW_H_