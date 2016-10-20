#include "stdafx.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "AgentsHandler.h"
#include "WritingModule.h"
#include "Engine.h"
#include "FoodHandler.h"
#include "Agent.h"
#include "Brain.h"
#include "Settings.h"
#include "Helpers.h"

#include <random>

AgentsHandler::AgentsHandler()
{
}


AgentsHandler::~AgentsHandler()
{
}

void AgentsHandler::initialize()
{
	m_agents = new std::vector<Agent*>();

	for (int i = 0; i < NB_OF_AGENTS; ++i)
	{
		addRandomAgent();
	}
}

void AgentsHandler::update()
{
	std::vector<float> inputs;

	std::vector<Agent*>::iterator it;
	for (it = m_agents->begin(); it != m_agents->end();)
	{
		Agent* agent = (*it);
		bool dead = false;

		if (!agent->m_isHerbivore)
		{
			std::vector<Agent*>::iterator it3;
			for (it3 = m_agents->begin(); it3 != m_agents->end();)
			{
				Agent* otherAgent = (*it3);

				if (otherAgent != agent && otherAgent->m_isHerbivore)
				{

					float dist = (agent->m_position - otherAgent->m_position).norm();

					if (dist < AGENT_DEFAULT_SIZE)
					{
						otherAgent->m_health -= 0.1f;

						if (otherAgent->m_health < 0)
							agent->m_food += KILL_FOOD;
					}

				}

				++it3;
			}
		}

		if (agent->m_health <= 0.f)
		{
			dead = true;
		}

		if (dead)
		{
			delete * it;
			it = m_agents->erase(it);

			continue;
		}

		if (agent->m_food > FOOD_COST_TO_REPRODUCE)
		{
			addAgentReplica(agent);
			addAgentReplica(agent);

			agent->m_food -= FOOD_COST_TO_REPRODUCE;
		}

		float soundSensor = 0.0f; // detect sound


		float isNearestCarnivore = 0.0f;
		float isNearestDistance = 200.0f;

		std::vector<Agent*>::iterator it2;
		for (it2 = m_agents->begin(); it2 != m_agents->end();)
		{
			Agent* otherAgent = (*it2);
			if (otherAgent != agent)
			{
				float dist = (agent->m_position - otherAgent->m_position).norm();

				if (dist < DISTANCE_SOUND)
				{
					soundSensor += dist / DISTANCE_SOUND;
				}

				if (dist < isNearestDistance)
				{
					if (otherAgent->m_isHerbivore)
						isNearestCarnivore = 0.0f;
					else
						isNearestCarnivore = 1.0f;

					isNearestDistance = dist;
				}
			}
			++it2;
		}


		float smellSensor = 0.0f; // detect smell
		/*for (int i = 0; i < (int)Engine::Instance().getFoodHandler().getFoods()->size(); ++i)
		{
			Eigen::Vector2f foodPosition = Engine::Instance().getFoodHandler().getCenterPositionFromIndex(i);
			
			float distance = (agent->m_position - foodPosition).norm();

			if (distance < DISTANCE_SMELL)
			{
				smellSensor += distance / DISTANCE_SMELL;
			}
		}*/

		float health = agent->m_health;
		float numberOfFoodsOnAgent = agent->m_food;
		float numberOfFoodsOnTile = Engine::Instance().getFoodHandler().getFoodAt((int)agent->m_position.x(), (int)agent->m_position.y());
		float positionX = agent->m_position.x() / WIDTH;
		float positionY = agent->m_position.y() / HEIGHT;

		inputs.push_back(Helpers::cap(soundSensor));
		//inputs.push_back(Engine::cap(smellSensor));
		inputs.push_back(Helpers::cap(health));
		inputs.push_back(Helpers::cap(numberOfFoodsOnAgent));
		inputs.push_back(Helpers::cap(numberOfFoodsOnTile));
		inputs.push_back(Helpers::cap(positionX));
		inputs.push_back(isNearestCarnivore);
		inputs.push_back(Helpers::cap(positionY));
		inputs.push_back(1);

		agent->m_brain->setInput(inputs);
		agent->m_brain->tick();
		agent->update();

		inputs.clear();

		++it;
	}

	if (m_agents->size() < NB_OF_AGENTS)
	{
		int rand = std::rand() % 100;
		int nb = NB_OF_AGENTS - m_agents->size();

		for (int i = 0; i < nb; i++)
		{
			/*if (rand < 49 && m_agents->size() != 0)
				addCrossoverAgent();
			else*/
				addRandomAgent();
		}
	}
}

void AgentsHandler::draw()
{
	for each (Agent* agent in *m_agents)
	{
		filledCircleRGBA(Engine::Instance().getSdlRenderer(), (int)agent->m_position.x(), (int)agent->m_position.y(), (int)agent->m_size, agent->m_r, agent->m_g, agent->m_b, agent->m_a);

		if (agent->m_isHerbivore)
			circleRGBA(Engine::Instance().getSdlRenderer(), (int)agent->m_position.x(), (int)agent->m_position.y(), (int)agent->m_size, 0, 255, 0, 255);
		else
			circleRGBA(Engine::Instance().getSdlRenderer(), (int)agent->m_position.x(), (int)agent->m_position.y(), (int)agent->m_size, 255, 0, 0, 255);

		if (Engine::Instance().isDebugDrawEnabled())
		{
			// health bar
			int full_x = (int)agent->m_position.y() - 10;
			int empty_x = (int)agent->m_position.y() + 10;
			int bar_size = empty_x - full_x;
			float current_x = empty_x - (bar_size * agent->m_health);
			boxRGBA(Engine::Instance().getSdlRenderer(), (int)agent->m_position.x() + (int)agent->m_size / 2 + 8, (int)current_x, (int)agent->m_position.x() + (int)agent->m_size / 2 + 9, empty_x, 0xFF, 0x00, 0x00, SDL_ALPHA_OPAQUE);

			// food counter
			int nbOfFood = (int)(agent->m_food * 100.0f);

			SDL_Rect textPosition;
			textPosition.x = (int)agent->m_position.x() - 9;
			textPosition.y = (int)agent->m_position.y() - 6;
			textPosition.w = 19;
			textPosition.h = 15;

			Engine::Instance().getWritingModule().writeNumber(nbOfFood, textPosition.x, textPosition.y, textPosition.w, textPosition.h);
		}
	}
}

void AgentsHandler::close()
{
	m_agents->clear();
}

void AgentsHandler::addRandomAgent()
{
	int position_x = std::rand() % WIDTH;
	int position_y = std::rand() % HEIGHT;
	float size = AGENT_DEFAULT_SIZE;
	float speed = AGENT_DEFAULT_SPEED;
	bool isHerbivore = (rand() % 2 == 1) ? true : false;

	int r = std::rand() % 256;
	int g = std::rand() % 256;
	int b = std::rand() % 256;
	int a = 255;

	m_agents->push_back(new Agent(Eigen::Vector2f(position_x, position_y), isHerbivore, speed, size, r, g, b, a));
}

void AgentsHandler::addCrossoverAgent()
{
	std::vector<Agent*>::iterator it;
	Agent* best = (*m_agents)[rand() % m_agents->size()];
	Agent* secondBest = nullptr;

	bool herbivoreOnly = best->m_isHerbivore; // will we reproduce two herbivores or two carnivores ( no mix )

	bool stop = false;
	while (!stop)
	{
		secondBest = (*m_agents)[rand() % m_agents->size()];

		if (secondBest->m_isHerbivore == herbivoreOnly)
			stop = true;
	}

	int random;
	// choose mommy & daddy
	for (it = m_agents->begin(); it != m_agents->begin();)
	{
		Agent* agent = (*it);
		random = rand() % 10;

		if (random < 3 && (*it)->m_isHerbivore == herbivoreOnly)
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
	int position_x = std::rand() % WIDTH;
	int position_y = std::rand() % HEIGHT;
	float size = daddy->m_size;
	float speed = daddy->m_speed;

	int r = daddy->m_r;
	int g = daddy->m_g;
	int b = daddy->m_b;

	bool isHerbivore = daddy->m_isHerbivore;

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

	m_agents->push_back(new Agent(Eigen::Vector2f(position_x, position_y), isHerbivore, speed, size, r, g, b, 255));
}

void AgentsHandler::addAgentReplica(Agent* model)
{
	//printf("REPLICA\n");
	m_agents->push_back(new Agent(model));
}