#pragma once
#include <Eigen/Dense>
#include <vector>

#include "Perceptron.h"

class Brain
{
public:
	Brain();
	Brain(Brain*, Brain*);
	~Brain();

	void setInput(std::vector<float> inputs);
	void feedForward();
	std::vector<float> getOutputs();

private:
	int inputLayerSize;
	int outputLayerSize;
	int hiddenLayerSize;
	int connectionFromHiddentoOutput;
	
	std::vector<Perceptron*> inputPerceptrons;
	std::vector<Perceptron*> hiddenPerceptrons;
	std::vector<Perceptron*> outputPerceptrons;
};

