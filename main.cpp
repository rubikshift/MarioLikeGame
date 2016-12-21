#include "sdl.h"
#include "basicSDLFunctions.h"
#include "mario.h"
#include "enemy.h"
#include "level.h"
#include "game.h"
#include "levelList.h"

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
	SDL_Texture *scrtex, *marioTexture, *enemyTexture, *tileTexture;
	SDL_Window *window;
	SDL_Renderer *renderer;	

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

	mario* player = new mario(marioTexture, 16, 16, renderer);
	level** gameLevels = new level*[levels.size];
	for (int i = 0; i < levels.size; i++)
		gameLevels[i] = new level(levels[i], player, tileTexture, enemyTexture, renderer);
	
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
			gameLevels[actualLevel] = new level(levels[actualLevel], player, tileTexture, enemyTexture, renderer);
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

			sprintf(text, "Czas trwania = %.1lf s  Liczba zyc = %d  Liczba monet = %d", gameLevels[actualLevel]->getTime(), player->lives, player->coins);
			drawString(screen, 10, 10, text, charset);
			sprintf(text, "Esc - wyjscie, n - nowa gra, l - wczytaj gre, s - zapisz gre");
			drawString(screen, 10, 26, text, charset);
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			reload = gameLevels[actualLevel]->update(delta);
			gameLevels[actualLevel]->render(renderer);
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
							newGame(player); 
							actualLevel = 0; 
							reload = true;
							break;
						case SDLK_s: break;
						case SDLK_l: break;
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

	SDL_Quit();
	return 0;
};
