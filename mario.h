#pragma once
#include "sdl.h"
#include "velocity.h"
#include "point.h"
#include "collision.h"
#include "tile.h"
#include "enemy.h"

enum marioInfo
{
	animationFramesCount = 10,
	numberOfLives = 3,
	moveSpeed = 80, //pixel per second
	moveRightStartFrame = 0,
	moveRightEndFrame = 3,
	moveLeftStartFrame = 4,
	moveLeftEndFrame = 7,
	jumpLeftFrame = 9,
	jumpRightFrame = 8,
	maxJumpHeight = 100 //pixels
};

class mario
{
	private:
		
		int actualFrame;
		bool isJumping;
		SDL_Rect animationFrames[animationFramesCount];
		SDL_Texture* spriteSheet;
		velocity characterVelocity;
		point position;
	public:
		mario(char* spriteSheetFile, int spriteWidth, int spriteHeight, point start, SDL_Renderer* renderer);
		~mario();

		int lives;
		int coins;

		void moveRight();
		void moveLeft();
		void jump();
		void stop();
		void update(double timeElapsed);
		void render(SDL_Renderer* renderer);
		point getPosition();
		collision checkTileCollisions(tile** tiles, int count);
		bool checkEnemyCollisions(enemy** enemies, int count);
		int getWidth();
		int getHeight();
};