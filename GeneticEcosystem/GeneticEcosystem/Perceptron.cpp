#include "stdafx.h"
#include "Perceptron.h"
#include "Synapse.h"

#include <cmath>

Perceptron::Perceptron()
{
}


Perceptron::~Perceptron()
{
	for (std::vector< Synapse* >::iterator it = m_synapses.begin(); it != m_synapses.end(); ++it)
	{
		delete (*it);
	}
	m_synapses.clear();
}

void Perceptron::addSynapse(Perceptron* perceptron, float weight, int index)
{
	Synapse* synapse = new Synapse(perceptron, weight, index);
	m_synapses.push_back(synapse);
}

void Perceptron::addSynapse(Synapse* synapse)
{
	m_synapses.push_back(synapse);
}

void Perceptron::computeState()
{
	float sum = 0.f;

	for each (Synapse* input in m_synapses)
	{
		sum += input->m_inputPerceptron->m_state * input->m_weight;
	}

	m_state = thresholdFunc(sum);
}

float Perceptron::thresholdFunc(float sum)
{
	return (1.f / (1.f + std::exp(-m_beta * sum))) * 2.0f - 1.0f; // range [-1;1]
}