#pragma once
#include "point.h"
#include "tile.h"
#include "mario.h"
#include "enemy.h"
#include "coin.h"


class level
{
	private:
		char* fileName;
		int levelWidth;
		int enemiesCount;
		int tilesCount;
		int coinsCount;
		double loadedLevelTime;
		double levelTime;
		
		tile** tiles;
		enemy** enemies;
		coin** coins;
		mario* player;
		point startPosition;
		point endPosition;

		void load(SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Texture* coinTexture, SDL_Renderer* renderer);
	
	public:
		level(char* fileName, mario* player, SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Texture* coinTexture, SDL_Renderer* rendere);
		~level();
		void render(SDL_Renderer* renderer, int x);
		bool update(double timeElapsed);
		double getTime();
		bool isFinished();
		void start();
		int getWidth();

};