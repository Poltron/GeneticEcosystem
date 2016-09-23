#include "stdafx.h"
#include "Joker.h"
#include "Engine.h"

#include "Settings.h"

#include <list>

Joker::Joker()
{
	int posx = rand() % WIDTH;
	int posy = rand() % HEIGHT;
	m_position = Eigen::Vector2f(posx, posy);
}


Joker::~Joker()
{
}

void Joker::update(std::vector<Agent*>* agents)
{
	m_timerFocusTarget -= 0.01f;

	if (m_timerFocusTarget < 0)
	{
		float closestTarget = (float)m_distanceView;

		std::vector<Agent*>::iterator it;
		for (it = agents->begin(); it != agents->end();)
		{
			Agent* agent = (*it);

			float result = (m_position - agent->m_position).norm();
			if (result < (float)m_distanceView)
			{
				if (m_target != agent && result < closestTarget)
				{
					m_target = agent;
					m_distanceToTarget = result;
					m_timerFocusTarget = m_timerFocusTargetValue;
				}
			}

			++it;
		}
	}

	if (m_target == nullptr)
	{
		int randomTarget = rand() % agents->size();
		m_target = (*agents)[randomTarget];
	}

	m_velocity = (m_target->m_position - m_position).normalized() * (float)Engine::Instance().getElapsedTime() * m_speed;

	move(m_velocity);

	if (m_alreadyAgainstTheWall > 1)
	{
		m_target = (*agents)[rand() % agents->size()];
	}

}


void Joker::move(Eigen::Vector2f direction)
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

void Joker::resetTarget()
{
	m_timerFocusTarget = 0.0f;
	m_target = nullptr;
}