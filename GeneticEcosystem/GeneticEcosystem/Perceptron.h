#pragma once
#include <vector>

class Brain;
class Synapse;

class Perceptron
{
public:
	Perceptron();
	~Perceptron();

	void addSynapse(Perceptron* synapse, float weight, int index);
	void addSynapse(Synapse* synapse);

	// output state
	float m_state = 0.0f;

	// width of the threshold function
	float m_beta = 1.0f;

	// Performs feedword algorithm : compute the Perceptron outpute State using the inputs and their weight
	void computeState();

	// Sigmoid threshold function that modifies the input to an output
	float thresholdFunc(float input);

	// list of all inputs for the Perceptron
	std::vector<Synapse*> m_synapses;
};
