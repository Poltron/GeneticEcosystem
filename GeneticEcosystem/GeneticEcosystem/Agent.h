#pragma once

#include <Eigen/Dense>

class Brain;

class Agent
{
public:
	Agent(Eigen::Vector2f position, float speed, float size, int r, int g, int b, int a);
	Agent::Agent(Brain* brain, Eigen::Vector2f position, float speed, float size, int r, int g, int b, int a);
	~Agent();

	Eigen::Vector2f m_position;
	Eigen::Vector2f m_velocity;
	float m_size;
	float m_speed;

	int m_r;
	int m_g;
	int m_b;
	int m_a;

	int m_age;
	double m_timeCumulElapsed;
	float m_alreadyAgainstTheWall;

	Brain* m_brain;

	void update();
	void move(Eigen::Vector2f);
private:
};

