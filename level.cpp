#include <stdlib.h>
#include "level.h"
#define BUFF_SIZE 8
#define LEVEL_LINES 15
#define SECOND 1
#define MINUTE 60
#define HOUR 3600

level::level(char* fileName, mario* player, SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* renderer)
{
	int length = strlen(fileName) + 1;
	this->fileName = new char[length];
	memcpy(this->fileName, fileName, length);
	this->player = player;
	tilesCount = 0;
	enemiesCount = 0;
	this->load(tileTexture, enemyTexture, renderer);
	//this->player->setPosition(this->startPosition);
}

level::~level()
{
	for (int i = 0; i < this->tilesCount; i++)
		delete this->tiles[i];
	for (int i = 0; i < this->enemiesCount; i++)
		delete this->enemies[i];
	delete[] this->tiles;
	delete[] this->enemies;
}

void level::render(SDL_Renderer* renderer)
{
	this->player->render(renderer);
	for (int i = 0; i < this->enemiesCount; i++)
		this->enemies[i]->render(renderer);
	for (int i = 0; i < this->tilesCount; i++)
		this->tiles[i]->render(renderer);
}

void level::update(double timeElapsed)
{
	this->player->checkCollisions(this->enemies, this->enemiesCount);
	collision tileCollisionType = this->player->checkCollisions(this->tiles, this->tilesCount);
	this->player->update(timeElapsed, tileCollisionType);
	for (int i = 0; i < this->enemiesCount; i++)
		this->enemies[i]->update(timeElapsed);
	this->levelTime -= timeElapsed;
}

void level::load(SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Renderer* renderer)
{
	FILE* fileStream = fopen(this->fileName, "r");
	point p, q;
	char buff[BUFF_SIZE];
	char* data[LEVEL_LINES];
	int tileIterator = 0, enemyIterator = 0;

	if (fileStream != NULL)
	{
		fscanf(fileStream, "%s", buff);
		fscanf(fileStream, "%s", buff);
		this->levelTime = atoi(buff);
		fscanf(fileStream, "%s", buff);
		if (buff[0] == 's' || buff[0] == 'S')
			levelTime *= SECOND;
		else if (buff[0] == 'm' || buff[0] == 'M')
			levelTime *= MINUTE;
		else if (buff[0] == 'h' || buff[0] == 'H')
			levelTime *= HOUR;

		fscanf(fileStream, "%s", buff);
		fscanf(fileStream, "%s", buff);
		this->levelWidth = atoi(buff);

		for (int i = 0; i < LEVEL_LINES; i++)
		{
			data[i] = new char[this->levelWidth];
			fscanf(fileStream, "%s", data[i]);
		}

		for (int y = 0; y < LEVEL_LINES; y++)
			for (int x = 0; x < this->levelWidth; x++)
			{
				if (data[y][x] == 'g' || data[y][x] == 'G' || data[y][x] == 'c' || data[y][x] == 'C'
					|| data[y][x] == 'p' || data[y][x] == 'P')
					this->tilesCount++;
				else if (data[y][x] != 's' && data[y][x] != 'S' && data[y][x] != 'm' && data[y][x] != 'M'
					&& ((data[y][x] >= 'a' && data[y][x] <= 'z') || (data[y][x] >= 'A' && data[y][x] <= 'Z')))
					this->enemiesCount++;
			}
		this->tiles = new tile*[this->tilesCount];
		this->enemies = new enemy*[this->enemiesCount];
		for (int y = 0; y < LEVEL_LINES; y++)
			for (int x = 0; x < this->levelWidth; x++)
			{
				p = {(double)x * (double)tileWidth, (double)y * (double)tileHeight};
				if (data[y][x] == 'g' || data[y][x] == 'G')
				{
					tiles[tileIterator] = new tile(tileTexture, groundTile, tileTextureWidth, tileTextureWidth, p, renderer);
					tileIterator++;
				}
				else if (data[y][x] == 'p' || data[y][x] == 'P')
				{
					tiles[tileIterator] = new tile(tileTexture, platformTile, tileTextureWidth, tileTextureWidth, p, renderer);
					tileIterator++;
				}
				else if (data[y][x] == 'c' || data[y][x] == 'C')
				{
					tiles[tileIterator] = new tile(tileTexture, coinTile, tileTextureWidth, tileTextureWidth, p, renderer);
					tileIterator++;
				}
				else if (data[y][x] == 's' || data[y][x] == 'S')
					this->startPosition = p;
				else if (data[y][x] == 'm' || data[y][x] == 'M')
					this->endPosition = p;
				else if ((data[y][x] >= 'a' && data[y][x] <= 'z') || (data[y][x] >= 'A' && data[y][x] <= 'Z'))
				{
					for (int i = x; i < this->levelWidth; i++)
						if (data[y][x] == data[y][i])
							q = {(double)i * (double)tileWidth, y * (double)tileHeight};
					this->enemies[enemyIterator] = new enemy(enemyTexture, enemySpriteWidth, enemySpriteHeight, p, q, renderer);
					enemyIterator++;
				}
			}
	}
	fclose(fileStream);
}