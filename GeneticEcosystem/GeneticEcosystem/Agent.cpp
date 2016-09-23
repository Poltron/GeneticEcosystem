#include "stdafx.h"
#include "Agent.h"
#include "Brain.h"
#include "Engine.h"
#include "Settings.h"

Agent::Agent(Eigen::Vector2f position, float speed, float size, int r, int g, int b, int a)
{
	m_position = position;
	m_size = size;
	m_speed = speed;

	m_age = 0;

	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;

	m_brain = new Brain();
	m_alreadyAgainstTheWall = 0;
}

Agent::Agent(Brain* brain, Eigen::Vector2f position, float speed, float size, int r, int g, int b, int a)
{
	m_position = position;
	m_size = size;
	m_speed = speed;

	m_age = 0;

	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;

	m_brain = brain;
}

Agent::~Agent()
{
}

void Agent::update()
{
	std::vector<float> outputs = m_brain->getOutputs();
	
	Eigen::Vector2f direction = (Eigen::Vector2f(outputs[0] * WIDTH, outputs[1] * HEIGHT)).normalized();
	double speed = Engine::Instance().getElapsedTime() * (double)m_speed;
	m_velocity = direction * (float)speed;

	move(m_velocity);

	m_timeCumulElapsed += Engine::Instance().getElapsedTime();
	
	if (m_timeCumulElapsed > 1)
	{
		m_age++;
		m_timeCumulElapsed = 0;
	}
}

void Agent::move(Eigen::Vector2f direction)
{
	m_position += direction;
	bool onWall = false;

	if (m_position.x() + m_size > WIDTH)
	{
		onWall = true;
		m_position = Eigen::Vector2f(WIDTH - m_size, m_position.y());
	}
	else if (m_position.x() - m_size < 0)
	{
		onWall = true;
		m_position = Eigen::Vector2f(0 + m_size, m_position.y());
	}

	if (m_position.y() + m_size > HEIGHT)
	{
		onWall = true;
		m_position = Eigen::Vector2f(m_position.x(), HEIGHT - m_size);
	}
	else if (m_position.y() - m_size < 0)
	{
		onWall = true;
		m_position = Eigen::Vector2f(m_position.x(), 0 + m_size);
	}

	if (onWall)
		m_alreadyAgainstTheWall += (float)Engine::Instance().getElapsedTime();
	else
		m_alreadyAgainstTheWall = 0;
}