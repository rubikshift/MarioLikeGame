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

		double levelTime;
		
		

		void load(SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* renderer);
	
	public:
	tile** tiles;
	enemy** enemies;
	mario* player;
	point startPosition;
	point endPosition;
	int enemiesCount;
	int tilesCount;
		level(char* fileName, mario* player, SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* rendere);
		~level();
		void render(SDL_Renderer* renderer);
		void update(double timeElapsed);

};