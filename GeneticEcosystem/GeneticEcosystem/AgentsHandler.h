#pragma once

#include <vector>

class Agent;
class Joker;

class AgentsHandler
{
public:
	AgentsHandler();
	~AgentsHandler();

	void initialize(SDL_Renderer* renderer);
	void update();
	void draw();
	void close();

	void addRandomAgent();
	void addCrossoverAgent();
	void addCrossoverAgent(Agent* mommy, Agent* daddy);
	void speedUpAgents(float multiplier);
	void slowDownAgents(float diviser);

private:
	std::vector<Agent*>* m_agents;
	SDL_Renderer* m_renderer;
	std::vector<Joker*>* m_jokers;

	int speedMultiplierStack = 1;
};

