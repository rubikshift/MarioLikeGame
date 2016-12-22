#pragma once
#include "point.h"
#include "sdl.h"

enum coinInfo
{
	coinMaxPopHeight = 50, //pixels
	coinSpeed = 95 // pixels per second
};
enum coinDefaultSize
{
	coinTextureWidth = 16, //pixels
	coinTextureHeight = 16, //pixels
	coinWidth = 32, //pixels
	coinHeight = 32 //pixels
};
class coin
{
	public:
		SDL_Rect rect;
		SDL_Texture* texture;
		point position;
		bool visible;
		double distance;

		coin(SDL_Texture* loadedTexture, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer);
		void render(SDL_Renderer* renderer, int x);
		void disable();
		void enable();
		void update(double timeElapsed);
};