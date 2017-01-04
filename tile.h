#pragma once
#include "sdl.h"
#include "point.h"

enum tileType
{
	groundTile, 
	platformTile,
	coinTile
};

enum tileDefaultSize
{
	tileTextureWidth = 16, //pixels
	tileTextureHeight = 16, //pixels
	tileWidth = 32, //pixels
	tileHeight = 32 //pixels
};

class tile
{
	private:
		SDL_Rect rect;
		SDL_Texture* texture;
		point position;
		bool visible;

	public:
		tileType type;

		tile(SDL_Texture* loadedTexture, tileType type, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer);
		void render(SDL_Renderer* renderer, int x);
		void disable();
		bool isVisible();
		double getWidth();
		double getHeight();
		point getPosition();
};