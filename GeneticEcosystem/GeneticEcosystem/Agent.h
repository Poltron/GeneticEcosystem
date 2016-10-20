#pragma once

#include <Eigen/Dense>

class Brain;

class Agent
{
public:
	Agent(Agent* model);
	Agent(Eigen::Vector2f position, bool isHerbivore, float speed, float size, int r, int g, int b, int a);
	Agent(Brain* brain, Eigen::Vector2f position, bool isHerbivore, float speed, float size, int r, int g, int b, int a);
	~Agent();

	Eigen::Vector2f m_position;
	Eigen::Vector2f m_velocity;
	float m_size;
	float m_speed;
	float m_angle;

	int m_r;
	int m_g;
	int m_b;
	int m_a;

	int m_age;

	bool m_isHerbivore;

	float m_food;
	float m_health;

	double m_timeCumulElapsed;

	Brain* m_brain;

	void update();
	void move(Eigen::Vector2f);
private:

	void updateTimers();
	void consumeFood();
};

