#include "stdafx.h"
#include "Synapse.h"


Synapse::Synapse()
{
}

Synapse::Synapse(Perceptron* perceptron, float weight)
{
	m_inputPerceptron = perceptron;
	m_weight = weight;
}

Synapse::~Synapse()
{
}
