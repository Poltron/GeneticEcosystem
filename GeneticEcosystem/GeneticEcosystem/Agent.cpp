#include "stdafx.h"
#include "Agent.h"
#include "Brain.h"
#include "Engine.h"
#include "FoodHandler.h"
#include "Settings.h"

Agent::Agent(Eigen::Vector2f position, bool isHerbivore, float speed, float size, int r, int g, int b, int a)
{
	m_position = Eigen::Vector2f(position.x(), position.y());
	m_size = size;
	m_speed = speed;

	m_age = 0;
	m_isHerbivore = isHerbivore;

	m_food = 1.0f;
	m_health = 1.0f;
	
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;

	m_brain = new Brain();
}

Agent::Agent(Brain* brain, Eigen::Vector2f position, bool isHerbivore, float speed, float size, int r, int g, int b, int a)
{
	m_position = Eigen::Vector2f(position.x(), position.y());
	m_size = size;
	m_speed = speed;

	m_age = 0;
	m_isHerbivore = isHerbivore;

	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;

	m_brain = brain;
}

Agent::~Agent()
{
	delete m_brain;
}

void Agent::update()
{
	std::vector<float> outputs = m_brain->getOutputs();
	
	Eigen::Vector2f direction = Eigen::Vector2f(outputs[0], outputs[1]).normalized();
	if (direction.x() != direction.x() || direction.y() != direction.y()) // anti erreur float undefined 1.#IND0000
	{
		direction = Eigen::Vector2f(0, 0);
	}

	double speed = Engine::Instance().getElapsedTime() * (double)m_speed;
	m_velocity = direction * (float)speed;

	move(m_velocity);

	if (m_isHerbivore)
	{
		m_food += Engine::Instance().getFoodHandler().consumeFoodAt((int)m_position.x(), (int)m_position.y(), FOOD_GRAB_PER_SEC * (float)Engine::Instance().getElapsedTime());
	}

	consumeFood();

	updateTimers();
}

void Agent::updateTimers()
{
	// age
	m_timeCumulElapsed += Engine::Instance().getElapsedTime();

	if (m_timeCumulElapsed > 1)
	{
		m_age++;
		m_timeCumulElapsed = 0;
	}
}

void Agent::consumeFood()
{
	float foodNeeded = (float)((double)FOOD_CONSUMPTION_PER_SEC * Engine::Instance().getElapsedTime());
	if (m_food >= foodNeeded)
	{
		m_food -= foodNeeded;
	}
	else
	{
		m_health -= foodNeeded - m_food;
		m_food = 0.f;
	}
}

void Agent::move(Eigen::Vector2f direction)
{
	m_position += direction;

	if (m_position.x() + m_size > WIDTH)
	{
		m_position = Eigen::Vector2f(WIDTH - m_size, m_position.y());
	}
	else if (m_position.x() - m_size < 0)
	{
		m_position = Eigen::Vector2f(0 + m_size, m_position.y());
	}

	if (m_position.y() + m_size > HEIGHT)
	{
		m_position = Eigen::Vector2f(m_position.x(), HEIGHT - m_size);
	}
	else if (m_position.y() - m_size < 0)
	{
		m_position = Eigen::Vector2f(m_position.x(), 0 + m_size);
	}
}