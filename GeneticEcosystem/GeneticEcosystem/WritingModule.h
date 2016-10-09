#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

class WritingModule
{
public:
	WritingModule();
	~WritingModule();

	void initialize();
	void writeNumber(int number, int x, int y, int w, int h);
	void writeStatic();
	void close();

private:
	SDL_Surface* numberSurfaces[10];
	SDL_Texture* numberTextures[10];

	TTF_Font* m_font;
};

