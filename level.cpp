#include <stdlib.h>
#include "level.h"
#define BUFF_SIZE 64
#define LEVEL_LINES 15
#define SECOND 1
#define MINUTE 60
#define HOUR 3600

level::level(char* fileName, mario* player, SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Texture* coinTexture, SDL_Renderer* renderer)
{
	int length = strlen(fileName) + 1;
	this->fileName = new char[length];
	memcpy(this->fileName, fileName, length);
	this->player = player;
	this->tilesCount = 0;
	this->enemiesCount = 0;
	this->coinsCount = 0;
	this->load(tileTexture, enemyTexture, coinTexture, renderer);
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

void level::render(SDL_Renderer* renderer, int x)
{

	this->player->render(renderer , x);
	for (int i = 0; i < this->enemiesCount; i++)
		this->enemies[i]->render(renderer, x);
	for (int i = 0; i < this->tilesCount; i++)
		this->tiles[i]->render(renderer, x);
	for (int i = 0; i < this->coinsCount; i++)
		this->coins[i]->render(renderer, x);
}

bool level::update(double timeElapsed)
{
	point p = this->player->getPosition();
	if (this->player->checkCollisions(this->enemies, this->enemiesCount) || p.y >= SCREEN_HEIGHT || levelTime <= 0)
	{
		this->player->coins = 0;
		this->player->lives--;
		return true;
	}
	collision tileCollisionType = this->player->checkCollisions(this->tiles, this->tilesCount, this->coins, this->coinsCount);
	this->player->update(timeElapsed, tileCollisionType);
	for (int i = 0; i < this->enemiesCount; i++)
		this->enemies[i]->update(timeElapsed);
	for (int i = 0; i < this->coinsCount; i++)
		this->coins[i]->update(timeElapsed);
	this->levelTime -= timeElapsed;
	return false;
}

void level::load(SDL_Texture* tileTexture, SDL_Texture* enemyTexture, SDL_Texture* coinTexture, SDL_Renderer* renderer)
{
	FILE* fileStream = fopen(this->fileName, "r");
	point p, q;
	char buff[BUFF_SIZE];
	char* data[LEVEL_LINES];
	int tileIterator = 0, enemyIterator = 0, coinIterator = 0;

	if (fileStream != NULL)
	{
		fscanf(fileStream, "%s", buff);
		fscanf(fileStream, "%s", buff);
		this->loadedLevelTime = atoi(buff);
		fscanf(fileStream, "%s", buff);
		if (buff[0] == 's' || buff[0] == 'S')
			this->loadedLevelTime *= SECOND;
		else if (buff[0] == 'm' || buff[0] == 'M')
			this->loadedLevelTime *= MINUTE;
		else if (buff[0] == 'h' || buff[0] == 'H')
			this->loadedLevelTime *= HOUR;

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
				if (data[y][x] == 'c' || data[y][x] == 'C')
					this->coinsCount++;
				else if (data[y][x] != 's' && data[y][x] != 'S' && data[y][x] != 'm' && data[y][x] != 'M'
					&& ((data[y][x] >= 'a' && data[y][x] <= 'z') || (data[y][x] >= 'A' && data[y][x] <= 'Z')))
					this->enemiesCount++;
			}
		this->tiles = new tile*[this->tilesCount];
		this->enemies = new enemy*[this->enemiesCount/2];
		this->coins = new coin*[this->coinsCount];
		for (int y = 0; y < LEVEL_LINES; y++)
			for (int x = 0; x < this->levelWidth; x++)
			{
				p = {(double)x * (double)tileWidth, (double)y * (double)tileHeight};
				if (data[y][x] == 'g' || data[y][x] == 'G')
				{
					tiles[tileIterator] = new tile(tileTexture, groundTile, tileTextureWidth, tileTextureHeight, p, renderer);
					tileIterator++;
				}
				else if (data[y][x] == 'p' || data[y][x] == 'P')
				{
					tiles[tileIterator] = new tile(tileTexture, platformTile, tileTextureWidth, tileTextureHeight, p, renderer);
					tileIterator++;
				}
				else if (data[y][x] == 'c' || data[y][x] == 'C')
				{
					tiles[tileIterator] = new tile(tileTexture, coinTile, tileTextureWidth, tileTextureHeight, p, renderer);
					tileIterator++;
					coins[coinIterator] = new coin(coinTexture, coinTextureWidth, coinTextureHeight, p, renderer);
					coinIterator++;
				}
				else if (data[y][x] == 's' || data[y][x] == 'S')
					this->startPosition = p;
				else if (data[y][x] == 'm' || data[y][x] == 'M')
					this->endPosition = p;
				else if ((data[y][x] >= 'a' && data[y][x] <= 'z') || (data[y][x] >= 'A' && data[y][x] <= 'Z'))
				{
					for (int i = x + 1; i < this->levelWidth; i++)
						if (data[y][x] == data[y][i])
						{
							q = {(double)i * (double)tileWidth, y * (double)tileHeight};
							this->enemies[enemyIterator] = new enemy(enemyTexture, enemySpriteWidth, enemySpriteHeight, p, q, renderer);
							enemyIterator++;
						}
				}
			}
		fclose(fileStream);
	}
	enemiesCount = enemyIterator;
	
}

double level::getTime()
{ return this->levelTime; }

bool level::isFinished()
{
	point p = this->player->getPosition();
	if (p.x >= this->endPosition.x)
		return true;
	else return false;
}

void level::start()
{ 
	this->levelTime = this->loadedLevelTime;
	this->player->setPosition(this->startPosition);
	this->player->isJumping = false;
}

int level::getWidth()
{ return this->levelWidth; }

void level::saveToFile(FILE* fileStream)
{
	fwrite(&this->levelTime, sizeof(levelTime), 1, fileStream);
	double x;
	for (int i = 0; i < enemiesCount; i++)
	{
		x = this->enemies[i]->getPosition().x;
		fwrite(&x, sizeof(x), 1, fileStream);
		fwrite(&this->enemies[i]->actualVelocity.x, sizeof(this->enemies[i]->actualVelocity.x), 1, fileStream);
	}
	for (int i = 0; i < this->tilesCount; i++)
		if (this->tiles[i]->isVisible() != true)
			fwrite(&i, sizeof(i), 1, fileStream);
}

void level::loadFromFile(FILE* fileStream)
{
	fread(&this->levelTime, sizeof(levelTime), 1, fileStream);
	int q;
	point p;
	for (int i = 0; i < enemiesCount; i++)
	{
		p = this->enemies[i]->getPosition();
		fread(&p.x, sizeof(p.x), 1, fileStream);
		fread(&this->enemies[i]->actualVelocity.x, sizeof(this->enemies[i]->actualVelocity.x), 1, fileStream);
		this->enemies[i]->setPosition(p);
	}
	while (feof(fileStream) == 0)
	{
		fread(&q, sizeof(q), 1, fileStream);
		if(q < tilesCount && q >= 0)
			tiles[q]->disable();
	}
}