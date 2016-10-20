#include "stdafx.h"
#include "Agent.h"
#include "Brain.h"
#include "Engine.h"
#include "FoodHandler.h"
#include "Settings.h"
#include "Helpers.h"

#include <Eigen/src/Geometry/Transform.h>

Agent::Agent(Eigen::Vector2f position, bool isHerbivore, float speed, float size, int r, int g, int b, int a)
{
	m_position = Eigen::Vector2f(position.x(), position.y());
	m_size = size;
	m_speed = speed;

	m_age = 0;
	m_isHerbivore = isHerbivore;

	m_food = 1.0f;
	m_health = 1.0f;
	m_angle = Helpers::randomRangeFloat((float)-M_PI, (float)M_PI);

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

Agent::Agent(Agent* model)
{
	m_position = Eigen::Vector2f(Helpers::randomRangeInt(20, WIDTH - 20), Helpers::randomRangeInt(20, HEIGHT - 20));
	m_angle = Helpers::randomRangeFloat((float)-M_PI, (float)M_PI);

	m_size = model->m_size;
	m_speed = model->m_speed;

	m_age = 0;
	m_isHerbivore = model->m_isHerbivore;

	m_r = model->m_r;
	m_g = model->m_g;
	m_b = model->m_b;
	m_a = model->m_a;

	m_brain = new Brain(model->m_brain);
}

Agent::~Agent()
{
	delete m_brain;
}

void Agent::update()
{
	std::vector<float> outputs = m_brain->getOutputs();
	
/*	Eigen::Vector2f v(m_size / 2, 0);
	Eigen::Rotation2D<float> rotation(m_angle + M_PI / 2.0f);
	v = rotation * v;

	Eigen::Transform<float, 2, Eigen::Affine> transform;
	transform = Eigen::Translation2f(v) * Eigen::Rotation2D<float>(m_angle + M_PI / 2.0f) * Eigen::Scaling(1.f);
	
	Eigen::Vector2f w1p = m_position + v; //wheel positions
	Eigen::Vector2f w2p = m_position - v;

	float BW1 = m_speed * outputs[0];
	float BW2 = m_speed * outputs[1];

	//move bots
	Eigen::Vector2f velocity = w2p - m_position;
	rotation = Eigen::Rotation2D<float>(-BW1);
	velocity = rotation * velocity;
	m_position = w2p - velocity;
	m_angle -= BW1;

	if (m_angle<-M_PI)
		m_angle = M_PI - (-M_PI - m_angle);

	velocity = m_position - w1p;
	rotation = Eigen::Rotation2D<float>(BW2);
	velocity = rotation * velocity;
	m_position = w1p + velocity;
	m_angle += BW2;

	if (m_angle>M_PI)
		m_angle = -M_PI + (m_angle - M_PI);

	//wrap around the map
	if (m_position.x() < 0)
		m_position = Eigen::Vector2f(WIDTH + m_position.x(), m_position.y());

	if (m_position.x() >= WIDTH)
		m_position = Eigen::Vector2f(m_position.x() - WIDTH, m_position.y());

	if (m_position.y() < 0)
		m_position = Eigen::Vector2f(m_position.x(), HEIGHT + m_position.y());

	if (m_position.y() >= HEIGHT)
		m_position = Eigen::Vector2f(m_position.x(), m_position.y() - HEIGHT);
		*/
	Eigen::Vector2f direction = Eigen::Vector2f(outputs[0] * 2.0f - 1.0f, outputs[1] * 2.0f - 1.0f).normalized();
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

	//wrap around the map
	if (m_position.x() < 0)
		m_position = Eigen::Vector2f(WIDTH + m_position.x(), m_position.y());

	if (m_position.x() >= WIDTH)
		m_position = Eigen::Vector2f(m_position.x() - WIDTH, m_position.y());

	if (m_position.y() < 0)
		m_position = Eigen::Vector2f(m_position.x(), HEIGHT + m_position.y());

	if (m_position.y() >= HEIGHT)
		m_position = Eigen::Vector2f(m_position.x(), m_position.y() - HEIGHT);

}