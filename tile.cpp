#include "tile.h"
#include "basicSDLFunctions.h"

tile::tile(SDL_Texture* loadedTexture, tileType type, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer)
{
	this->position = position;
	this->texture = loadedTexture;
	this->visible = true;
	this->rect.x = (int)type * textureWidth;
	this->rect.y = 0;
	this->rect.h = textureHeight;
	this->rect.w = textureWidth;
}

void tile::render(SDL_Renderer* renderer)
{
	if (!visible)
		return;
	SDL_Rect sprite;
	sprite.x = this->position.x;
	sprite.y = this->position.y;
	sprite.w = this->rect.w * 2;
	sprite.h = this->rect.h * 2;
	SDL_RenderCopy(renderer, this->texture, &(this->rect), &sprite);
}

int tile::getWidth()
{ return this->rect.w*2; }

int tile::getHeight()
{ return this->rect.h*2; }

point tile::getPosition()
{ return this->position; }

bool tile::isVisible()
{ return this->visible; }

void tile::disable()
{ this->visible = false; }