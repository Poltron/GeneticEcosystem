#include "stdafx.h"
#include "Brain.h"
#include "Settings.h"
#include "Synapse.h"

Brain::Brain()
{
	m_perceptrons.resize(BRAIN_SIZE);

	for (int i = 0; i < BRAIN_SIZE; ++i)
	{
		Perceptron perceptron(this);
		m_perceptrons[i] = perceptron;
	}
}

Brain::Brain(Brain* model)
{
	m_perceptrons.resize(BRAIN_SIZE);

	for (int i = 0; i < BRAIN_SIZE; ++i)
	{
		Perceptron perceptron(this, model->m_perceptrons[i]);
		m_perceptrons[i] = perceptron;
	}
}

Brain::Brain(Brain* mommy, Brain* daddy)
{
	/*Brain* model = (rand() % 2 == 1) ? mommy : daddy;*/

}

void Brain::setInput(std::vector<float> inputs)
{
	for (int i = 0; i < BRAIN_INPUT_LAYER_SIZE; ++i)
	{
		m_perceptrons[i].m_output = inputs[i];
	}
}

void Brain::tick()
{
	// on calcule l'état de chaque output à partir des inputs
	for (int i = BRAIN_INPUT_LAYER_SIZE; i < BRAIN_SIZE; ++i)
	{
		Perceptron* perceptron = &m_perceptrons[i];

		float accumulator = 0;
		for (int j = 0; j < SYNAPSE_PER_PERCEPTRON; j++)
		{
			int index_input = perceptron->inputs[j].m_index;
			float val = m_perceptrons[index_input].m_output;

			accumulator += val * perceptron->inputs[j].m_weight;
		}

		accumulator *= perceptron->m_gw; // on applique le multiplicateur global
		accumulator += perceptron->m_bias; // on applique le bias

		accumulator = 1.0f / (1.0f + exp(-accumulator)); // on passe par la sigmoid

		perceptron->m_target = accumulator; // l'accumulator est l'output ""pur""
		perceptron->m_output += (perceptron->m_target - perceptron->m_output)*perceptron->m_kp;
		// l'output final est la différence entre l'accumulator et l'output précédent, passé par le multiplicateur, le DAMPER
	}

	// on backup l'output du tick précédent
	/*for (int i = 0; i<BRAIN_SIZE; i++){
		m_perceptrons[i].m_old_output = m_perceptrons[i].m_output;
	}*/

	// on fait avancer l'output vers l'output target par un tick
	/*for (int i = BRAIN_INPUT_LAYER_SIZE; i<BRAIN_SIZE; i++) {
		Perceptron* perceptron = &m_perceptrons[i];
		perceptron->m_output = perceptron->m_output + (perceptron->m_target - perceptron->m_output)*perceptron->m_kp;
	}*/
};

std::vector<float> Brain::getOutputs()
{
	std::vector<float> outputs;
	for (int i = BRAIN_SIZE - BRAIN_OUTPUT_LAYER_SIZE; i < BRAIN_SIZE; ++i)
	{
		outputs.push_back(m_perceptrons[i].m_output);
	}

	return outputs;
}

Brain::~Brain()
{
	m_perceptrons.clear();
}