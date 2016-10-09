#include "stdafx.h"
#include "Brain.h"
#include "Settings.h"
#include "Synapse.h"

Brain::Brain()
{

	for (int i = 0; i < BRAIN_INPUT_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();
		inputPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < BRAIN_HIDDEN_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < SYNAPSE_PER_PERCEPTRON; ++j)
		{
			Synapse* synapse = new Synapse();
			
			int randomInputPerceptron = rand() % BRAIN_INPUT_LAYER_SIZE;
			synapse->m_inputPerceptron = inputPerceptrons[randomInputPerceptron];

			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / SYNAPSE_WEIGHT_RANGE);
			synapse->m_weight = r - (SYNAPSE_WEIGHT_RANGE / 2.f);

			perceptron->addSynapse(synapse);
		}

		hiddenPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < BRAIN_OUTPUT_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < SYNAPSE_PER_PERCEPTRON; ++j)
		{
			Synapse* synapse = new Synapse();

			int randomInputPerceptron = rand() % BRAIN_HIDDEN_LAYER_SIZE + BRAIN_INPUT_LAYER_SIZE;
			synapse->m_index = randomInputPerceptron;

			if (randomInputPerceptron >= BRAIN_INPUT_LAYER_SIZE)
				synapse->m_inputPerceptron = hiddenPerceptrons[randomInputPerceptron - BRAIN_INPUT_LAYER_SIZE];
			else
				synapse->m_inputPerceptron = inputPerceptrons[randomInputPerceptron];

			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / SYNAPSE_WEIGHT_RANGE);
			synapse->m_weight = r - (SYNAPSE_WEIGHT_RANGE / 2.f);
			perceptron->addSynapse(synapse);
		}

		outputPerceptrons.push_back(perceptron);
	}
}

Brain::Brain(Brain* mommy, Brain* daddy)
{
	Brain* model = (rand() % 2 == 1) ? mommy : daddy;

	for (int i = 0; i < BRAIN_INPUT_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();
		inputPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < BRAIN_HIDDEN_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < SYNAPSE_PER_PERCEPTRON; ++j)
		{
			Synapse* synapse = new Synapse();
			
			int index = model->hiddenPerceptrons[i]->m_synapses[j]->m_index;
			synapse->m_inputPerceptron = inputPerceptrons[index];

			float r = model->hiddenPerceptrons[i]->m_synapses[j]->m_weight;
			synapse->m_weight = r;

			perceptron->addSynapse(synapse);
		}

		hiddenPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < BRAIN_OUTPUT_LAYER_SIZE; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < SYNAPSE_PER_PERCEPTRON; ++j)
		{
			Synapse* synapse = new Synapse();
			synapse->m_inputPerceptron = hiddenPerceptrons[j + i];

			int randomInputPerceptron = model->outputPerceptrons[i]->m_synapses[j]->m_index;
			synapse->m_index = randomInputPerceptron;

			if (randomInputPerceptron >= BRAIN_INPUT_LAYER_SIZE)
				synapse->m_inputPerceptron = hiddenPerceptrons[randomInputPerceptron - BRAIN_INPUT_LAYER_SIZE];
			else
				synapse->m_inputPerceptron = inputPerceptrons[randomInputPerceptron];

			float r = model->outputPerceptrons[i]->m_synapses[j]->m_weight;
			synapse->m_weight = r;

			perceptron->addSynapse(synapse);
		}

		outputPerceptrons.push_back(perceptron);
	}
}

void Brain::setInput(std::vector<float> inputs)
{
	int length = 0;
	if (inputs.size() <= inputPerceptrons.size())
		length = inputs.size();
	else
		length = inputPerceptrons.size();

	for (int i = 0; i < length; ++i)
	{
		inputPerceptrons[i]->m_state = inputs[i];
	}
}

void Brain::feedForward()
{
	for each (Perceptron* p in hiddenPerceptrons)
	{
		p->computeState();
	}

	for each (Perceptron* p in outputPerceptrons)
	{
		p->computeState();
	}
};

std::vector<float> Brain::getOutputs()
{
	std::vector<float> outputs;

	for each (Perceptron* perceptron in outputPerceptrons)
	{
		outputs.push_back(perceptron->m_state);
	}

	return outputs;
}

Brain::~Brain()
{
	for (std::vector< Perceptron* >::iterator it = outputPerceptrons.begin(); it != outputPerceptrons.end(); ++it)
	{
		delete (*it);
	}
	outputPerceptrons.clear();

	for (std::vector< Perceptron* >::iterator it = hiddenPerceptrons.begin(); it != hiddenPerceptrons.end(); ++it)
	{
		delete (*it);
	}
	hiddenPerceptrons.clear();

	for (std::vector< Perceptron* >::iterator it = inputPerceptrons.begin(); it != inputPerceptrons.end(); ++it)
	{
		delete (*it);
	}
	inputPerceptrons.clear();
}