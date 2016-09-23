#pragma once
#include <Eigen/Dense>
#include <vector>

#include "Agent.h"

class Joker
{
	public:
		Joker();
		~Joker();
	
		void update(std::vector<Agent*>* agents);
	
		Eigen::Vector2f m_position;
		Eigen::Vector2f m_velocity;
		
		Agent* m_target;
		float m_distanceToTarget;
		float m_speed = 25.0f;
		float m_size = 10.0f;
		int m_distanceView = 100;

		void move(Eigen::Vector2f direction);
		void resetTarget();
	
		float m_alreadyAgainstTheWall = 0;

	private:
		float m_timerFocusTarget = 0.0f;
		float m_timerFocusTargetValue = 5.0f;
};

