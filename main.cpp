#include "sdl.h"
#include "basicSDLFunctions.h"
#include "mario.h"
#include "enemy.h"
#include "level.h"
#include "levelList.h"

const char* saveFile = "save.txt";

void saveGame(mario* player, level* gameLevel, int actualLevel)
{
	int isJumping = player->isJumping;
	FILE* fileStream = fopen(saveFile, "wb");
	int buffInt[] = {actualLevel, player->lives, player->coins, isJumping};
	double buffDouble[] = {player->startHeight, player->actualHeight, player->getPosition().x, player->getPosition().y};
	fwrite(buffInt, sizeof(int), sizeof(buffInt)/sizeof(int), fileStream);
	fwrite(buffDouble, sizeof(double), sizeof(buffDouble) / sizeof(double), fileStream);
	gameLevel->saveToFile(fileStream);
	fclose(fileStream);
}
void loadGame(mario* player, level** gameLevels, int &actualLevel)
{
	int isJumping;
	double x, y;
	FILE* fileStream = fopen(saveFile, "rb");
	if (fileStream == NULL)
		return;
	fread(&actualLevel, sizeof(actualLevel), 1, fileStream);
	fread(&player->lives, sizeof(player->lives), 1, fileStream);
	fread(&player->coins, sizeof(player->coins), 1, fileStream);
	fread(&isJumping, sizeof(isJumping), 1, fileStream);
	fread(&player->startHeight, sizeof(player->startHeight), 1, fileStream);
	fread(&player->actualHeight, sizeof(player->actualHeight), 1, fileStream);
	fread(&x, sizeof(x), 1, fileStream);
	fread(&y, sizeof(y), 1, fileStream);
	gameLevels[actualLevel]->loadFromFile(fileStream);
	if (isJumping == 0)
		player->isJumping = false;
	else
		player->isJumping = true;
	player->setPosition({x, y});
	fclose(fileStream);
}

void centerCamera(SDL_Rect* camera, int levelWidth, int x)
{
	camera->x = (x + (int)marioWidth / 2) - SCREEN_WIDTH / 2;
	if (camera->x < 0)
		camera->x = 0;
	if (camera->x > levelWidth - camera->w)
		camera->x = levelWidth - camera->w;
}
// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv)
{
	int t1, t2, rc, actualLevel = 0;
	bool quit  = false, reload = false;
	double delta, worldTime;
	levelList levels;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex, *marioTexture, *enemyTexture, *tileTexture, *coinTexture;
	SDL_Window *window;
	SDL_Renderer *renderer;	
	SDL_Rect camera;
	camera.x = 0;
	camera.y = 0;
	camera.h = SCREEN_HEIGHT;
	camera.w = SCREEN_WIDTH;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
	
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	if(rc != 0) 
	{
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Michal Krakowiak 165596");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) 
	{
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	int black = SDL_MapRGB(screen->format, 0, 0, 0);
	int skyBlue = SDL_MapRGB(screen->format, 119, 181, 254);

	marioTexture = loadTexture("spritesheet.bmp", renderer);
	enemyTexture = loadTexture("eti.bmp", renderer);
	tileTexture = loadTexture("tiles.bmp", renderer);
	coinTexture = loadTexture("coin.bmp", renderer);

	mario* player = new mario(marioTexture, 16, 16, renderer);
	level** gameLevels = new level*[levels.size];
	for (int i = 0; i < levels.size; i++)
		gameLevels[i] = new level(levels[i], player, tileTexture, enemyTexture, coinTexture, renderer);

	gameLevels[actualLevel]->start();
	t1 = SDL_GetTicks();
	while(!quit)
	{
		SDL_RenderClear(renderer);
		t2 = SDL_GetTicks();

		delta = (t2 - t1) * 0.001;
		t1 = t2;
		if (reload)
		{
			delete gameLevels[actualLevel];
			gameLevels[actualLevel] = new level(levels[actualLevel], player, tileTexture, enemyTexture, coinTexture, renderer);
			gameLevels[actualLevel]->start();
			reload = false;
			continue;
		}
		if (gameLevels[actualLevel]->isFinished() && actualLevel < levels.size - 1)
		{
			actualLevel++;
			gameLevels[actualLevel]->start();
		}
		else if ((gameLevels[actualLevel]->isFinished() && actualLevel == levels.size - 1) || player->lives == 0)
		{
			SDL_FillRect(screen, NULL, black);
			sprintf(text, "Koniec Gry!");
			drawString(screen, 10, 10, text, charset);
			sprintf(text, "Esc - wyjscie, n - nowa gra, l - wczytaj gre");
			drawString(screen, 10, 26, text, charset);
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		}
		else
		{
			SDL_FillRect(screen, NULL, skyBlue);
			centerCamera(&camera, gameLevels[actualLevel]->getWidth()*(int)tileWidth, player->getPosition().x);
			sprintf(text, "Czas trwania = %.1lf s  Liczba zyc = %d  Liczba monet = %d", gameLevels[actualLevel]->getTime(), player->lives, player->coins);
			drawString(screen, 10, 10, text, charset);
			sprintf(text, "Esc - wyjscie, n - nowa gra, l - wczytaj gre, s - zapisz gre");
			drawString(screen, 10, 26, text, charset);
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			reload = gameLevels[actualLevel]->update(delta);
			gameLevels[actualLevel]->render(renderer, camera.x);
		}

		SDL_RenderPresent(renderer);
		while(SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE: quit = true; break;
						case SDLK_UP: player->jump(); break;
						case SDLK_RIGHT: player->moveRight(); break;
						case SDLK_LEFT: player->moveLeft(); break;
						case SDLK_n: 
							player->lives = (int)numberOfLives;
							player->coins = 0;
							actualLevel = 0; 
							reload = true;
							break;
						case SDLK_s: saveGame(player, gameLevels[actualLevel], actualLevel); break;
						case SDLK_l: loadGame(player, gameLevels, actualLevel); break;
					}
					break;
				case SDL_KEYUP:
					player->stop();
					break;
				case SDL_QUIT:
					quit = true;
					break;
			};
		};
	};

	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(marioTexture);
	SDL_DestroyTexture(enemyTexture);
	SDL_DestroyTexture(tileTexture);
	delete player;
	for (int i = 0; i < levels.size; i++)
		delete gameLevels[i];
	delete[] gameLevels;
	SDL_Quit();
	return 0;
};
