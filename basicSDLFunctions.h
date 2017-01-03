#pragma once
#include "sdl.h"

void drawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
void drawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void drawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void drawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void drawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
SDL_Texture* loadTexture(char* filename, SDL_Renderer* renderer, int colorKey);