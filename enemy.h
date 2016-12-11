#pragma once
#include "sdl.h"
#include "velocity.h"
#include "point.h"

enum enemyInfo
{
	enemyAnimationFramesCount = 2,
	enemyMoveSpeed = 60 //pixels per second
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
		velocity actualVelocity;
	public:
		enemy(char* spriteSheetFile, int spriteWidth, int spriteHeight, point start, point end, SDL_Renderer* renderer);
		~enemy();
		void update(double timeElapsed);
		void render(SDL_Renderer* renderer);
};