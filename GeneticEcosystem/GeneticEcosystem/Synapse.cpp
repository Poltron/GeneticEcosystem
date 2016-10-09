#include "stdafx.h"
#include "Synapse.h"


Synapse::Synapse()
{
}

Synapse::Synapse(Perceptron* perceptron, float weight, int index)
{
	m_inputPerceptron = perceptron;
	m_weight = weight;
	m_index = index;
}

Synapse::~Synapse()
{
	m_inputPerceptron = nullptr;
}
