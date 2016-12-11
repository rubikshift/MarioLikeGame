#pragma once
#include "sdl.h"
#include "point.h"

class tile
{
	private:
		SDL_Rect rect;
		SDL_Texture* texture;
		point position;

	public:
		tile(char* textureFile, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer);
		~tile();
		void render(SDL_Renderer* renderer);
};