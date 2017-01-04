#pragma once
#include "sdl.h"
#include "velocity.h"
#include "point.h"

enum enemyInfo
{
	enemyAnimationFramesCount = 3,
	enemyMoveSpeed = 60 //pixels per second
};

enum enemyDefaultSize
{
	enemySpriteWidth = 16, //pixels
	enemySpriteHeight = 16, //pixels
	enemyWidth = 32, //pixels
	enemyHeight = 32 //pixels
};

class enemy
{
	private:
		point start;
		point end;
		point position;
		int actualFrame;
		SDL_Rect animationFrames[enemyAnimationFramesCount];
		SDL_Texture* spriteSheet;

	public:
		velocity actualVelocity;

		enemy(SDL_Texture* loadedSprite, int spriteWidth, int spriteHeight, point start, point end, SDL_Renderer* renderer);
		void update(double timeElapsed);
		void render(SDL_Renderer* renderer, int x);
		point getPosition();
		void setPosition(point p);
		double getWidth();
		double getHeight();
};