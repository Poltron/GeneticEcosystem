#pragma once

class Perceptron;

// input data used for each Perceptron : an input perceptron and its weight
class Synapse
{
public:
	Perceptron* m_inputPerceptron;
	float m_weight;

	Synapse();
	Synapse(Perceptron* perceptron, float weight);
	~Synapse();
};
