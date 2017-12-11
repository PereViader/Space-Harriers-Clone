#pragma once

struct SDL_Texture;
struct SDL_Rect;

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

private:
	SDL_Texture* texture;
	SDL_Rect* section;
};