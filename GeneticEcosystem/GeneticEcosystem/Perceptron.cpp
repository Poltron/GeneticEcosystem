#include "stdafx.h"
#include "Perceptron.h"
#include "Synapse.h"
#include "Helpers.h"
#include "Brain.h"
#include <cmath>

Perceptron::Perceptron()
{
}

Perceptron::Perceptron(Brain* _brain, Perceptron model)
{
	brain = _brain;

	inputs.resize(SYNAPSE_PER_PERCEPTRON);

	for (int i = 0; i < SYNAPSE_PER_PERCEPTRON; ++i)
	{
		Synapse synapse(model.inputs[i].m_weight, model.inputs[i].m_index);
		inputs[i] = synapse;
	}

	m_kp = model.m_kp;
	m_gw = model.m_gw;
	m_bias = model.m_bias;

	m_output = 0.0f;
	m_old_output = 0.0f;
	m_target = 0.0f;
}

Perceptron::Perceptron(Brain* _brain)
{
	brain = _brain;

	inputs.resize(SYNAPSE_PER_PERCEPTRON);

	for (int i = 0; i < SYNAPSE_PER_PERCEPTRON; ++i)
	{
		Synapse synapse;
		inputs[i] = synapse;
	}

	m_kp = Helpers::randomRangeFloat(0.9f, 1.1f);
	m_gw = Helpers::randomRangeFloat(0.0f, 5.0f);
	m_bias = Helpers::randomRangeFloat(-2.0f, 2.0f);

	m_output = 0.0f;
	m_old_output = 0.0f;
	m_target = 0.0f;
}


Perceptron::~Perceptron()
{
	inputs.clear();
}