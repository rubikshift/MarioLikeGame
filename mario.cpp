#include "mario.h"
#include "basicSDLFunctions.h"

mario::mario(char* spriteSheetFile, int spriteWidth, int spriteHeight, point start, SDL_Renderer* renderer)
{
	this->position = start;
	this->coins = 0;
	this->lives = (int)numberOfLives;
	this->isJumping = false;
	this->actualFrame = 0;
	this->spriteSheet = LoadTexture(spriteSheetFile, renderer);
	for (int i = 0; i < (int)animationFramesCount; i++)
	{
		animationFrames[i].x = i*spriteWidth;
		animationFrames[i].y = 0;
		animationFrames[i].w = spriteWidth;
		animationFrames[i].h = spriteHeight;
	}
}

mario::~mario()
{
	SDL_DestroyTexture(this->spriteSheet);
}

void mario::moveRight()
{
	this->characterVelocity.x = (double)moveSpeed;
	if (this->isJumping)
		this->actualFrame = (int)jumpRightFrame;
	else
	{
		if (this->actualFrame >= (int)moveRightStartFrame && this->actualFrame < (int)moveRightEndFrame)
			this->actualFrame++;
		else
			this->actualFrame = (int)moveRightStartFrame + 1;
	}
}

void mario::moveLeft()
{
	this->characterVelocity.x = (-1) * (double)moveSpeed;
	if (this->isJumping)
		this->actualFrame = (int)jumpLeftFrame;
	else
	{
		if (this->actualFrame >= (int)moveLeftStartFrame && this->actualFrame < (int)moveLeftEndFrame)
			this->actualFrame++;
		else
			this->actualFrame = (int)moveLeftStartFrame + 1;
	}
}

void mario::stop()
{
	if (this->characterVelocity.x > 0)
	{
		if (this->isJumping)
			this->actualFrame = (int)jumpRightFrame;
		else
			this->actualFrame = (int)moveRightStartFrame;
	}
	else if (this->characterVelocity.x < 0)
	{
		if(this->isJumping)
			this->actualFrame = (int)jumpLeftFrame;
		else
			this->actualFrame = (int)moveLeftStartFrame;
	}
	this->characterVelocity.x = 0;
}

void mario::update(double timeElapsed)
{
	static int startHeight, actualHeight;
	
	this->position.x += this->characterVelocity.x * timeElapsed;
	this->position.y += characterVelocity.y * timeElapsed;
	
	if (isJumping)
		actualHeight = this->position.y;
	else
		startHeight = this->position.y;
	if (isJumping && (startHeight - actualHeight) >= (int)maxJumpHeight)
		this->characterVelocity.y = (double)moveSpeed;
	else if (isJumping && actualHeight >= startHeight)
	{
		this->characterVelocity.y = 0;
		this->isJumping = false;
		if (this->characterVelocity.x >= 0)
			this->actualFrame = (int)moveRightStartFrame;
		else if (this->characterVelocity.x < 0)
			this->actualFrame = (int)moveLeftStartFrame;
	}
}

void mario::jump()
{
	if (isJumping)
		return;
	else
	{
		this->characterVelocity.y = (-1) * (double)moveSpeed;
		this->isJumping = true;
		if (actualFrame >= (int)moveRightStartFrame && actualFrame <= (int)moveRightEndFrame)
			actualFrame = jumpRightFrame;
		else
			actualFrame = jumpLeftFrame;
	}
}

void mario::render(SDL_Renderer* renderer)
{
	SDL_Rect sprite;
	sprite.x = this->position.x;
	sprite.y = this->position.y;
	sprite.w = this->animationFrames[actualFrame].w;
	sprite.h = this->animationFrames[actualFrame].h;

	SDL_RenderCopy(renderer, this->spriteSheet, &(this->animationFrames[this->actualFrame]), &sprite);
}
