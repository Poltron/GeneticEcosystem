#pragma once
#include "SDL.h"
#include "SDL_ttf.h"

#include <map>
#include <string>

class WritingModule
{
public:
	WritingModule();
	~WritingModule();

	void initialize();
	void writeNumber(int number, int x, int y, int w, int h);
	void writeStatic(std::string text, int x, int y, int w, int h);
	void close();

private:
	SDL_Surface* numberSurfaces[10];
	SDL_Texture* numberTextures[10];

	std::map<std::string, SDL_Surface*> staticSurfaces;
	std::map<std::string, SDL_Texture*> staticTextures;

	TTF_Font* m_font;
};

