#pragma once

#include "SDL\include\SDL.h"
#include "Animation.h"

class Texture
{
public:
	Texture() :
		texture(nullptr),
		section({-1,-1,-1,-1})
	{
	}

	Texture(SDL_Texture * texture, const SDL_Rect& section) :
		texture(texture),
		section(section)
	{
	}

	Texture(SDL_Texture * texture) :
		texture(texture)
	{
		section.x = 0;
		section.y = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &section.w, &section.h);
	}


	~Texture() 
	{
		//outside code is responsible for managing texture deletion
	}

	SDL_Texture* GetTexture() const { return texture; }

	const SDL_Rect& GetSection() const { return section; }
	
	void UpdateTexture(const Animation& animation) {
		section = animation.GetFrame();
	}

	void UpdateTexture(const SDL_Rect& section) {
		this->section = section;
	}

private:
	SDL_Texture* texture;
	SDL_Rect section;
};

inline bool operator==(const SDL_Rect& a, const SDL_Rect& b)
{
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

inline bool operator==(const Texture& lhs, const Texture& rhs) {
	return lhs.GetTexture() == rhs.GetTexture() && lhs.GetSection() == rhs.GetSection();
}

inline bool operator!=(const Texture& lhs, const Texture& rhs) {
	return !operator==(lhs, rhs);
}