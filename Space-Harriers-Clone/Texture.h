#pragma once

#include "SDL\include\SDL.h"

class Texture
{
public:
	Texture(SDL_Texture * texture, SDL_Rect * section) :
		texture(texture),
		section(section)
	{
	}

	~Texture() 
	{
		//outside code is responsible for managing texture deletion
		delete section;
	}

	SDL_Texture* GetTexture() { return texture; }
	SDL_Rect* GetSection() { return section; }
	void SetSection(const SDL_Rect& newSection ) { (*section) = newSection; }

private:
	SDL_Texture* texture;
	SDL_Rect* section;
};