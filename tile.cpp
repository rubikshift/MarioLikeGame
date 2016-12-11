#include "tile.h"
#include "basicSDLFunctions.h"

tile::tile(char* textureFile, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer)
{
	this->position = position;
	this->texture = LoadTexture(textureFile, renderer);
}

tile::~tile()
{
	SDL_DestroyTexture(this->texture);
}

void tile::render(SDL_Renderer* renderer)
{
	SDL_Rect sprite;
	sprite.x = this->position.x;
	sprite.y = this->position.y;
	sprite.w = this->rect.w;
	sprite.h = this->rect.h;
	SDL_RenderCopy(renderer, this->texture, &(this->rect), &sprite);
}