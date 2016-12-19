#pragma once
#include "point.h"
#include "tile.h"
#include "mario.h"
#include "enemy.h"


class level
{
	private:
		char* fileName;
		int levelWidth;
		int enemiesCount;
		int tilesCount;
		double levelTime;
		
		tile** tiles;
		enemy** enemies;
		mario* player;
		point startPosition;
		point endPosition;

		void load(SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* renderer);
	
	public:
		level(char* fileName, mario* player, SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* rendere);
		~level();
		void render(SDL_Renderer* renderer);
		void update(double timeElapsed);
		double getTime();

};