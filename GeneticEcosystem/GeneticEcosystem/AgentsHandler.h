#pragma once

#include <vector>

class Agent;

class AgentsHandler
{
public:
	AgentsHandler();
	~AgentsHandler();

	void initialize();
	void update();
	void draw();
	void close();

	void addRandomAgent();
	void addAgentReplica(Agent* model);
	void addCrossoverAgent();
	void addCrossoverAgent(Agent* mommy, Agent* daddy);

private:
	std::vector<Agent*>* m_agents;
};

