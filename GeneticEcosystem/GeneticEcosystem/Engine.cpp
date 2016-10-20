#include "stdafx.h"
#include "SDL2_gfxPrimitives.h"

#include "Engine.h"
#include "WritingModule.h"
#include "AgentsHandler.h"
#include "FoodHandler.h"
#include "Settings.h"

#include <Eigen/Dense>

#include <iostream>
#include <ctime>

bool Engine::initialize(int width, int height)
{
	m_width = width;
	m_height = height;

	m_drawEnabled = true;
	m_debugDrawEnabled = true;

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

	m_writingModule = new WritingModule();
	m_writingModule->initialize();

	m_foodHandler = new FoodHandler();
	m_foodHandler->initialize();

	m_agentsHandler = new AgentsHandler();
	m_agentsHandler->initialize();

	m_timeMultiplier = 1.0f;

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
	m_foodHandler->close();
	m_agentsHandler->close();
	m_writingModule->close();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	m_window = NULL;
	m_renderer = NULL;

	SDL_Quit();
}

bool Engine::pollEvent()
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
			case SDLK_p:
				setPause(!m_isPaused);
				break;
			case SDLK_d:
				setDebugDraw(!m_debugDrawEnabled);
				break;
			case SDLK_r:
				reset();
				break;
			case SDLK_u:
				m_timeMultiplier *= 2.f;
				//printf("%f", m_timeMultiplier);
				break;
			case SDLK_i:
				m_timeMultiplier /= 2.f;
				//printf("%f", m_timeMultiplier);
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

	return false;
}

bool Engine::update()
{
	bool isQuitting = pollEvent();

	if (!m_isPaused)
	{
		m_agentsHandler->update();
		m_foodHandler->update();
	}

	return isQuitting;
}

void Engine::draw()
{
	m_foodHandler->draw();
	m_agentsHandler->draw();

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = WIDTH;
	rect.h = HEIGHT;

	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(m_renderer, &rect);

	drawFPSMeter();

	SDL_RenderPresent(m_renderer);
}

void Engine::drawControlPanel()
{
}

void Engine::clear()
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);
}

void Engine::reset()
{
	m_agentsHandler->close();
	m_foodHandler->close();

	m_agentsHandler->initialize();
	m_foodHandler->initialize();
}

void Engine::drawFPSMeter()
{
	double framePerSec = 1.0 / getRealElapsedTime();

	SDL_Rect textPosition;
	textPosition.x = WIDTH - 50;
	textPosition.y = 0;
	textPosition.w = 50;
	textPosition.h = 25;

	m_writingModule->writeNumber((int)framePerSec, textPosition.x , textPosition.y, textPosition.w, textPosition.h);
}

AgentsHandler& Engine::getAgentHandler()
{
	return *m_agentsHandler;
}

FoodHandler& Engine::getFoodHandler()
{
	return *m_foodHandler;
}

SDL_Renderer* Engine::getSdlRenderer()
{
	return m_renderer;
}
SDL_Window* Engine::getWindow()
{
	return m_window;
}

WritingModule& Engine::getWritingModule()
{
	return *m_writingModule;
}

void Engine::setDebugDraw(bool debug)
{
	m_debugDrawEnabled = debug;
}

bool Engine::isDebugDrawEnabled()
{
	return m_debugDrawEnabled;
}

const double Engine::getRealElapsedTime()
{
	return m_elapsedTime;
}

const double Engine::getElapsedTime()
{
	return m_elapsedTime * m_timeMultiplier;
}

void Engine::setDraw(bool enabled)
{
	m_drawEnabled = enabled;
}

void Engine::setPause(bool isPaused)
{
	m_isPaused = isPaused;
}

void Engine::saveEnvironment()
{
}

void Engine::loadEnvironment()
{
}