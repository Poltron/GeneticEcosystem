#pragma once
#include <Eigen/Dense>
#include <vector>

#include "Settings.h"
#include "Perceptron.h"

class Brain
{
public:
	Brain();
	Brain(Brain*);
	Brain(Brain*, Brain*);
	~Brain();

	void setInput(std::vector<float> inputs);
	void tick();
	std::vector<float> getOutputs();

	std::vector<Perceptron> m_perceptrons;
};

