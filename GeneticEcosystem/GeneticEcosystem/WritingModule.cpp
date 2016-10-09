#include "stdafx.h"
#include "WritingModule.h"
#include "Engine.h"


WritingModule::WritingModule()
{
}


WritingModule::~WritingModule()
{
}

void WritingModule::initialize()
{
	// Init SDL_ttf
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	m_font = TTF_OpenFont("arial.ttf", 12);
	if (!m_font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

	for (int i = 0; i < 10; ++i)
	{
		char text[2] = "";
		sprintf_s(text, "%d", i);
		SDL_Color black = { 0, 0, 0, 255 };
		numberSurfaces[i] = TTF_RenderText_Solid(m_font, text, black);
		numberTextures[i] = SDL_CreateTextureFromSurface(Engine::Instance().getSdlRenderer(), numberSurfaces[i]);
	}
}

void WritingModule::writeNumber(int number, int x, int y, int w, int h)
{
	char text[15] = "";
	sprintf_s(text, "%d", number);

	int nbOfDigit = 0;
	while (text[nbOfDigit] != '\0')
	{
		nbOfDigit++;
	}

	int widthPerDigit = w / nbOfDigit;
	int xDigit = x;
	for (int i = 0; i < nbOfDigit; ++i)
	{
		int number = text[i] - '0';

		SDL_Rect numberPosition;
		numberPosition.x = xDigit;
		numberPosition.y = y;
		numberPosition.w = widthPerDigit;
		numberPosition.h = h;
		
		if (number >= 0)
			SDL_RenderCopy(Engine::Instance().getSdlRenderer(), numberTextures[number], NULL, &numberPosition);

		xDigit += widthPerDigit;
	}
}

void WritingModule::writeStatic()
{

}

void WritingModule::close()
{
	for each(SDL_Surface* surface in numberSurfaces)
	{
		SDL_FreeSurface(surface);
	}

	for each(SDL_Texture* texture in numberTextures)
	{
		SDL_DestroyTexture(texture);
	}

	TTF_CloseFont(m_font);
	TTF_Quit();
}