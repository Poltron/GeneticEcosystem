#pragma once
#include <vector>

#include "Settings.h"

class Brain;
class Synapse;

class Perceptron
{
public:
	Perceptron();
	Perceptron(Brain*);
	Perceptron(Brain*, Perceptron);
	~Perceptron();

	std::vector<Synapse> inputs;

	float m_kp;
	float m_gw;
	float m_bias;

	float m_output = 0.0f;
	float m_old_output;
	float m_target;

private:
	Brain* brain;
};
