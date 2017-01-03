#include "mario.h"
#include "basicSDLFunctions.h"

mario::mario(SDL_Texture* loadedSpriteSheet, int spriteWidth, int spriteHeight, SDL_Renderer* renderer)
{
	this->coins = 0;
	this->lives = (int)numberOfLives;
	this->isJumping = false;
	this->isFalling = false;
	this->actualFrame = 0;
	this->spriteSheet = loadedSpriteSheet;
	for (int i = 0; i < (int)animationFramesCount; i++)
	{
		animationFrames[i].x = i*spriteWidth;
		animationFrames[i].y = 0;
		animationFrames[i].w = spriteWidth;
		animationFrames[i].h = spriteHeight;
	}
}

void mario::moveRight()
{
	this->characterVelocity.x = (double)moveSpeed;
	if (this->isJumping || this->isFalling)
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
	if (this->isJumping || this->isFalling)
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

void mario::update(double timeElapsed, collision collisionType)
{	
	if (this->isJumping)
		this->actualHeight = this->position.y;
	else
		this->startHeight = this->position.y;
	if ((this->isJumping && (this->startHeight - this->actualHeight) >= (double)maxJumpDistance) || !this->isJumping && collisionType == none)
		this->fall();
	if (collisionType == groundCollision)
		this->stopFalling();
	else if (collisionType == coinCollision)
		this->coins++;
	this->position.x += this->characterVelocity.x * timeElapsed;
	this->position.y += characterVelocity.y * timeElapsed;
}

void mario::fall()
{
	this->isJumping = false;
	this->isFalling = true;
	this->characterVelocity.y = (double)moveSpeed;
}

void mario::stopFalling()
{
	if(this->isJumping != true)
		this->characterVelocity.y = 0;
	if(this->isFalling)
	{
		this->isFalling = false;
		if (this->characterVelocity.x >= 0)
			this->actualFrame = (int)moveRightStartFrame;
		else if (this->characterVelocity.x < 0)
			this->actualFrame = (int)moveLeftStartFrame;
	}
	int i = (int)(this->position.y) / (int)marioHeight;
	this->position.y = (double)i * (double)marioHeight;
}

void mario::jump()
{
	if (isJumping || isFalling)
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

void mario::render(SDL_Renderer* renderer, int x)
{
	SDL_Rect sprite;
	sprite.x = this->position.x - x;
	sprite.y = this->position.y;
	sprite.w = (int)marioWidth;
	sprite.h = (int)marioHeight;

	SDL_RenderCopy(renderer, this->spriteSheet, &(this->animationFrames[this->actualFrame]), &sprite);
}

point mario::getPosition()
{ return this->position; }

bool mario::checkCollisions(enemy** enemies, int count)
{
	point enemyPosition;
	for (int i = 0; i < count; i++)
	{
		enemyPosition = enemies[i]->getPosition();
		if (this->position.x >= enemyPosition.x && this->position.x <= (enemyPosition.x + enemies[i]->getWidth())
			|| (this->position.x + this->getWidth()) >= enemyPosition.x && (this->position.x + this->getWidth()) <= (enemyPosition.x + enemies[i]->getWidth()))
			if ((this->position.y + this->getHeight()) >= enemyPosition.y && (this->position.y + this->getHeight()) <= (enemyPosition.y + enemies[i]->getHeight())
				|| this->position.y >= enemyPosition.y && this->position.y <= (enemyPosition.y + enemies[i]->getHeight()))
				return true;
	}
	return false;
}

collision mario::checkCollisions(tile** tiles, int count, coin** coins, int coinCount)
{
	point tilePosition;
	for (int i = 0; i < count; i++)
	{
		tilePosition = tiles[i]->getPosition();
		if (tiles[i]->isVisible() != true)
			continue;
		if ((this->position.y >= tilePosition.y && this->position.y <= (tilePosition.y + tiles[i]->getHeight()))
			|| ((this->position.y + this->getHeight()) > tilePosition.y && (this->position.y + this->getHeight()) < (tilePosition.y + tiles[i]->getHeight())))
		{
			if ((this->position.x + this->getWidth()) > tilePosition.x && (this->position.x + this->getWidth()) < (tilePosition.x + tiles[i]->getWidth()) && this->characterVelocity.x > 0)
				this->characterVelocity.x = 0;
			else if (this->position.x < (tilePosition.x + tiles[i]->getWidth()) && this->position.x > tilePosition.x && this->characterVelocity.x < 0)
				this->characterVelocity.x = 0;
		}
		if (this->position.x >= tilePosition.x && this->position.x <= (tilePosition.x + tiles[i]->getWidth())
			|| (this->position.x + this->getWidth()) > tilePosition.x && (this->position.x + this->getWidth()) < (tilePosition.x + tiles[i]->getWidth()))
		{
			if ((this->position.y + this->getHeight()) >= tilePosition.y && (this->position.y + this->getHeight()) <= (tilePosition.y + tiles[i]->getHeight()))
				return groundCollision;
			else if (this->position.y >= tilePosition.y && this->position.y <= (tilePosition.y + tiles[i]->getHeight()))
			{
				if (this->isJumping)
				{
					tiles[i]->disable();
					this->fall();
					if (tiles[i]->type == coinTile)
					{
						for (int j = 0; j < coinCount; j++)
							if (coins[j]->position == tilePosition && coins[j]->visible != true)
								coins[j]->enable();
						return coinCollision;
					}
					else return platformCollision;
				}
			}
		}
		
	}
	return none;
}

int mario::getWidth()
{ return (int)marioWidth; }

int mario::getHeight()
{ return (int)marioHeight; }

void mario::setPosition(point p)
{ this->position = p; }
