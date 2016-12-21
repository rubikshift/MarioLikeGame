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

enum marioDefaultSize
{
	marioSpriteWidth = 16, //pixels
	marioSpriteHeight = 16, //pixels
	marioWidth = 32, //pixels
	marioHeight = 32 //pixels
};

class mario
{
	private:
		
		int actualFrame;
		bool isJumping;
		bool isFalling;
		SDL_Rect animationFrames[animationFramesCount];
		SDL_Texture* spriteSheet;
		velocity characterVelocity;
		point position;
	public:
		int lives;
		int coins;

		mario(SDL_Texture* loadedSpriteSheet, int spriteWidth, int spriteHeight, SDL_Renderer* renderer);
		void moveRight();
		void moveLeft();
		void jump();
		void fall();
		void stop();
		void stopFalling();
		void update(double timeElapsed, collision type);
		void render(SDL_Renderer* renderer);
		point getPosition();
		void setPosition(point p);
		collision checkCollisions(tile** tiles, int count);
		bool checkCollisions(enemy** enemies, int count);
		int getWidth();
		int getHeight();
};