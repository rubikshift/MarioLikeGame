#include "coin.h"

coin::coin(SDL_Texture* loadedTexture, int textureWidth, int textureHeight, point position, SDL_Renderer* renderer)
{
	this->position = position;
	this->texture = loadedTexture;
	this->visible = false;
	this->rect.x = 0;
	this->rect.y = 0;
	this->rect.h = textureHeight;
	this->rect.w = textureWidth;
	this->distance = 0;
}

void coin::render(SDL_Renderer* renderer, int x)
{
	if (!(this->visible))
		return;
	SDL_Rect sprite;
	sprite.x = this->position.x - x;
	sprite.y = this->position.y;
	sprite.w = (int)coinWidth;
	sprite.h = (int)coinHeight;
	SDL_RenderCopy(renderer, this->texture, &(this->rect), &sprite);
}

void coin::disable()
{ this->visible = false; }

void coin::enable()
{ this->visible = true; }

void coin::update(double timeElapsed)
{
	if (!(this->visible))
		return;
	distance += timeElapsed * (double)coinSpeed;
	if(distance >= (double)coinMaxPopHeight)
		this->position.y += timeElapsed * (double)coinSpeed;
	else
		this->position.y -= timeElapsed * (double)coinSpeed;
	if (this->position.y >= SCREEN_HEIGHT)
		this->disable();
}