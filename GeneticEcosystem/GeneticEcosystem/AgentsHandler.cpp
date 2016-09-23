#include "stdafx.h"
#include "SDL2_gfxPrimitives.h"

#include "AgentsHandler.h"
#include "Agent.h"
#include "Joker.h"
#include "Brain.h"
#include "Settings.h"

#include <random>

AgentsHandler::AgentsHandler()
{
}


AgentsHandler::~AgentsHandler()
{
}

void AgentsHandler::initialize(SDL_Renderer* renderer)
{
	m_agents = new std::vector<Agent*>();
	m_renderer = renderer;

	for (int i = 0; i < NB_OF_AGENTS; ++i)
	{
		addRandomAgent();
	}

	m_jokers = new std::vector<Joker*>();
	for (int i = 0; i < NB_OF_JOKERS; ++i)
		m_jokers->push_back(new Joker());
}

void AgentsHandler::update()
{
	std::vector<Agent*>::iterator it;
	for (it = m_agents->begin(); it != m_agents->end();)
	{
		Agent* agent = (*it);
		
		if (agent->m_alreadyAgainstTheWall > 0.1f)
		{
			printf("agent death against wall\n");
			delete * it;
			it = m_agents->erase(it);

			continue;
		}

		Joker* joker = (*m_jokers)[0];
		float lesserDistance = 1000000000;
	
		for each (Joker* j in *m_jokers)
		{
			float distance = (j->m_position - agent->m_position).norm();

			if (distance < lesserDistance)
			{
				lesserDistance = distance;
				joker = j;
			}
		}

		// Is the agent touching the joker ? If it is, destroy it.
		float dist = std::powf((agent->m_position.x() - joker->m_position.x()), 2) + std::powf((agent->m_position.y() - joker->m_position.y()), 2);
		float radius = std::powf((agent->m_size + joker->m_size), 2);
		if (dist <= radius)
		{
			if (agent == joker->m_target)
			{
				joker->resetTarget();
			}

			printf("agent death against joker\n");
			delete * it;
			it = m_agents->erase(it);

			continue;
		}

		++it;
	}

	if (m_agents->size() < NB_OF_AGENTS)
	{
		int rand = std::rand() % 100;
		int nb = NB_OF_AGENTS - m_agents->size();

		for (int i = 0; i < nb; i++)
		{
			if (rand < 49 && m_agents->size() != 0)
				addCrossoverAgent();
			else
				addRandomAgent();
		}
	}

	for each (Joker* joker in *m_jokers)
	{
		joker->update(m_agents);
	}

	std::vector<float> inputs;

	for each (Agent* agent in *m_agents)
	{
		Joker* joker = (*m_jokers)[0];
		float lesserDistance = 1000000000;

		for each (Joker* j in *m_jokers)
		{
			float distance = (j->m_position - agent->m_position).norm();

			if (distance < lesserDistance)
			{
				lesserDistance = distance;
				joker = j;
			}
		}

		Eigen::Vector2f toJoker = joker->m_position - agent->m_position;
		Eigen::Vector2f toJokerNormalized = (joker->m_position - agent->m_position).normalized();
		inputs.push_back(toJokerNormalized.x());
		inputs.push_back(toJokerNormalized.y());
		inputs.push_back(toJoker.norm());
		inputs.push_back(agent->m_position.x() / WIDTH);
		inputs.push_back(agent->m_position.y() / HEIGHT);
		inputs.push_back(1);

		agent->m_brain->setInput(inputs);
		agent->m_brain->feedForward();
		agent->update();

		inputs.clear();
	}
}

void AgentsHandler::draw()
{
	for each (Agent* agent in *m_agents)
	{
		circleRGBA(m_renderer, (int)agent->m_position.x(), (int)agent->m_position.y(), (int)agent->m_size, agent->m_r, agent->m_g, agent->m_b, agent->m_a);
	}
	for each (Joker* joker in *m_jokers)
	{
		filledCircleRGBA(m_renderer, (int)joker->m_position.x(), (int)joker->m_position.y(), (int)joker->m_size, 0x00, 0xFF, 0x00, 0xFF);
		circleRGBA(m_renderer, (int)joker->m_position.x(), (int)joker->m_position.y(), joker->m_distanceView, 0x00, 0xFF, 0x00, 0xFF);
	}
}

void AgentsHandler::close()
{
	m_agents->clear();
	m_jokers->clear();
	m_renderer = nullptr;
}



void AgentsHandler::addRandomAgent()
{
	int position_x = std::rand() % 1024;
	int position_y = std::rand() % 768;
	float size = 10.f;
	float speed = 30.f * (float)speedMultiplierStack;//(float)(std::rand() % 30) * speedMultiplierStack;

	int r = std::rand() % 256;
	int g = std::rand() % 256;
	int b = std::rand() % 256;
	int a = 255;

	m_agents->push_back(new Agent(Eigen::Vector2f(position_x, position_y), speed, size, r, g, b, a));
}

void AgentsHandler::addCrossoverAgent()
{
	std::vector<Agent*>::iterator it;
	Agent* best = (*m_agents)[rand() % m_agents->size()];
	Agent* secondBest = (*m_agents)[rand() % m_agents->size()];

	int random;
	// choose mommy & daddy
	for (it = m_agents->begin(); it != m_agents->begin();)
	{
		Agent* agent = (*it);
		random = rand() % 10;

		if (random < 3)
		{
			if (best == nullptr)
				best = agent;
			else if (secondBest == nullptr)
				secondBest = agent;
			else if (best != nullptr && best->m_age < agent->m_age)
			{
				secondBest = best;
				best = agent;
			}
			else if (secondBest != nullptr && secondBest->m_age < agent->m_age)
			{
				secondBest = agent;
			}
		}
	}

	addCrossoverAgent(secondBest, best);
}
void AgentsHandler::addCrossoverAgent(Agent* mommy, Agent* daddy)
{
	int position_x = std::rand() % 1024;
	int position_y = std::rand() % 768;
	float size = daddy->m_size;
	float speed = daddy->m_speed;

	int r = daddy->m_r;
	int g = daddy->m_g;
	int b = daddy->m_b;

	if (rand() % 10 < 5)
		size = mommy->m_size;

	if (rand() % 10 < 5)
		speed = mommy->m_speed;

	if (rand() % 10 < 5)
	{
		r = mommy->m_r;
		g = mommy->m_g;
		b = mommy->m_b;
	}

	Brain* brain = new Brain(mommy->m_brain, daddy->m_brain);

	m_agents->push_back(new Agent(Eigen::Vector2f(position_x, position_y), speed, size, r, g, b, 255));
}


void AgentsHandler::speedUpAgents(float multiplier)
{
	for each (Agent* agent in *m_agents)
	{
		agent->m_speed *= multiplier;
	}

	for each (Joker* joker in *m_jokers)
	{
		joker->m_speed *= multiplier;
	}

	speedMultiplierStack *= 2;
}

void AgentsHandler::slowDownAgents(float diviser)
{
	for each (Agent* agent in *m_agents)
	{
		agent->m_speed /= diviser;
	}

	for each (Joker* joker in *m_jokers)
	{
		joker->m_speed /= diviser;
	}

	speedMultiplierStack /= 2;
}