#pragma once
#include "SDL.h"
#include "Windows.h"

#include <string>

class AgentsHandler;

class Engine
{
public:
	static Engine& Instance()
	{
		static Engine    instance;
		return instance;
	}

	bool initialize(int width, int height);
	bool start();
	void stop();
	void close();

	void resetAgents();

	const AgentsHandler* getAgentHandler();
	const SDL_Renderer* getSdlRenderer();

	const double getElapsedTime();

	// misc
	SDL_Texture* loadImage(std::string path);

private:
	SDL_Window* m_window = NULL;
	SDL_Renderer* m_renderer = NULL;
	AgentsHandler* m_agentsHandler = NULL;

	int m_width;
	int m_height;

	Engine() {}
	Engine(Engine const&);		   // Don't Implement.
	void operator=(Engine const&); // Don't implement

	bool update();
	void draw();
	void clear();

	void setDraw(bool);

	bool m_drawEnabled;

	double m_ticks;
	double m_elapsedTime;
};

