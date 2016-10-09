#pragma once
#include "SDL.h"
#include "Windows.h"

#include <string>

class FoodHandler; 
class AgentsHandler;
class WritingModule;

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

	void reset();

	WritingModule& getWritingModule();
	AgentsHandler& getAgentHandler();
	FoodHandler& getFoodHandler();
	SDL_Renderer* getSdlRenderer();
	SDL_Window* getWindow();

	const double getElapsedTime();
	const float getTimeMultiplier();

	bool isDebugDrawEnabled();

	static float cap(float f);

private:
	SDL_Window* m_window = NULL;
	SDL_Renderer* m_renderer = NULL;
	AgentsHandler* m_agentsHandler = NULL;
	FoodHandler* m_foodHandler = NULL;
	WritingModule* m_writingModule = NULL;

	int m_width;
	int m_height;

	Engine() {}
	Engine(Engine const&);		   // Don't Implement.
	void operator=(Engine const&); // Don't implement

	bool update();
	void draw();
	void clear();

	void setDraw(bool);
	void setDebugDraw(bool);
	
	void drawFPSMeter();
	void drawControlPanel();

	bool pollEvent();
	void setPause(bool);
	bool m_isPaused;
	
	bool m_drawEnabled;
	bool m_debugDrawEnabled;

	double m_ticks;
	double m_elapsedTime;
	double m_timeMultiplier;
};

