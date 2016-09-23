#include "stdafx.h"

#include "Engine.h"
#include "AgentsHandler.h"

#include <Eigen/Dense>

#include <iostream>
#include <ctime>

bool Engine::initialize(int width, int height)
{
	m_width = width;
	m_height = height;

	m_drawEnabled = true;

	//Create window
	m_window = SDL_CreateWindow("Genetic Ecosystem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_SHOWN);
	if (m_window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Create renderer for window
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	m_agentsHandler = new AgentsHandler();
	m_agentsHandler->initialize(m_renderer);

	std::srand((unsigned int)std::time(0));

	return true;
}

bool Engine::start()
{
	bool quit = false;

	m_ticks = SDL_GetTicks();
	float totaltime = 0.0f;

	while (!quit)
	{
		if (m_drawEnabled)
			clear();

		quit = update();
		
		if (m_drawEnabled)
			draw();
		
		const double ticks = SDL_GetTicks();
		m_elapsedTime = (double)((ticks - m_ticks) / 1000.0);
		m_ticks = ticks;
	}

	return true;
}

void Engine::close()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	m_window = NULL;
	m_renderer = NULL;

	SDL_Quit();
}

bool Engine::update()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return true;
						break;
					case SDLK_d:
						setDraw(!m_drawEnabled);
						break;
					case SDLK_r:
						resetAgents();
						break;
					case SDLK_o:
						m_agentsHandler->speedUpAgents(2.0f);
						printf("speed_up");
						break;
					case SDLK_p:
						m_agentsHandler->slowDownAgents(2.0f);
						printf("slow_down");
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				return true;
				break;
			default:
				break;
		}
	}

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = m_width;
	rect.h = m_height;

	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(m_renderer, &rect);

	m_agentsHandler->update();

	return false;
}

void Engine::draw()
{
	m_agentsHandler->draw();

	SDL_RenderPresent(m_renderer);
}

void Engine::clear()
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);
}

const AgentsHandler* Engine::getAgentHandler()
{
	return m_agentsHandler;
}
const SDL_Renderer* Engine::getSdlRenderer()
{
	return m_renderer;
}

void Engine::resetAgents()
{
	m_agentsHandler->close();
	m_agentsHandler->initialize(m_renderer);
}

SDL_Texture* Engine::loadImage(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s!\n", path.c_str());
		return nullptr;
	}

	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s!\n", path.c_str());
		return nullptr;
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

const double Engine::getElapsedTime()
{
	return m_elapsedTime;
}

void Engine::setDraw(bool enabled)
{
	m_drawEnabled = enabled;
}