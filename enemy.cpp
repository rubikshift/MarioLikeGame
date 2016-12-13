#include "enemy.h"
#include "basicSDLFunctions.h"

enemy::enemy(char* spriteSheetFile, int spriteWidth, int spriteHeight, point start, point end, SDL_Renderer* renderer)
{
	this->start = start;
	this->position = start;
	this->end = end;
	actualFrame = 0;
	this->actualVelocity.x = 0;
	this->actualVelocity.y = 0;
	this->spriteSheet = LoadTexture(spriteSheetFile, renderer);
	for (int i = 0; i < (int)enemyAnimationFramesCount; i++)
	{
		animationFrames[i].x = i*spriteWidth;
		animationFrames[i].y = 0;
		animationFrames[i].w = spriteWidth;
		animationFrames[i].h = spriteHeight;
	}
}

enemy::~enemy()
{
	SDL_DestroyTexture(this->spriteSheet);
}

void enemy::update(double timeElapsed)
{
	if (this->start.x >= this->position.x)
		this->actualVelocity.x = (int)enemyMoveSpeed;
	else if (this->end.x <= this->position.x)
		this->actualVelocity.x = (-1) * (int)enemyMoveSpeed;
	actualFrame++;
	actualFrame %= (int)enemyAnimationFramesCount;
	this->position.x += actualVelocity.x * timeElapsed;
}

void enemy::render(SDL_Renderer* renderer)
{
	SDL_Rect sprite;
	sprite.x = this->position.x;
	sprite.y = this->position.y;
	sprite.w = this->animationFrames[actualFrame].w;
	sprite.h = this->animationFrames[actualFrame].h;

	SDL_RenderCopy(renderer, this->spriteSheet, &(this->animationFrames[this->actualFrame]), &sprite);
}

point enemy::getPosition()
{ return this->position; }

int enemy::getWidth()
{ return this->animationFrames[this->actualFrame].w; }

int enemy::getHeight()
{ return this->animationFrames[this->actualFrame].h; }