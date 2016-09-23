#include "stdafx.h"
#include "Brain.h"
#include "Synapse.h"

Brain::Brain()
{
	inputLayerSize = 5;
	outputLayerSize = 2;
	hiddenLayerSize = 10;
	connectionFromHiddentoOutput = 6;

	for (int i = 0; i < inputLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();
		inputPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < hiddenLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < inputLayerSize; ++j)
		{
			Synapse* synapse = new Synapse();
			synapse->m_inputPerceptron = inputPerceptrons[j];
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.0f);
			synapse->m_weight = r - 1;
			perceptron->addSynapse(synapse);
		}

		hiddenPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < outputLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < connectionFromHiddentoOutput; ++j)
		{
			Synapse* synapse = new Synapse();
			synapse->m_inputPerceptron = hiddenPerceptrons[j + i];
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2.0f);
			synapse->m_weight = r - 1;
			perceptron->addSynapse(synapse);
		}

		outputPerceptrons.push_back(perceptron);
	}
}

Brain::Brain(Brain* mommy, Brain* daddy)
{
	inputLayerSize = 3;
	outputLayerSize = 2;
	hiddenLayerSize = 4;
	connectionFromHiddentoOutput = 3;

	for (int i = 0; i < inputLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();
		inputPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < hiddenLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < inputLayerSize; ++j)
		{
			Synapse* synapse = new Synapse();
			synapse->m_inputPerceptron = inputPerceptrons[j];

			float r = daddy->hiddenPerceptrons[i]->m_synapses[j]->m_weight;
			if (rand() % 10 < 5)
				r = mommy->hiddenPerceptrons[i]->m_synapses[j]->m_weight;

			synapse->m_weight = r;
			perceptron->addSynapse(synapse);
		}

		hiddenPerceptrons.push_back(perceptron);
	}

	for (int i = 0; i < outputLayerSize; ++i)
	{
		Perceptron* perceptron = new Perceptron();

		for (int j = 0; j < connectionFromHiddentoOutput; ++j)
		{
			Synapse* synapse = new Synapse();
			synapse->m_inputPerceptron = hiddenPerceptrons[j + i];

			float r = daddy->outputPerceptrons[i]->m_synapses[j]->m_weight;
			if (rand() % 10 < 5)
				r = mommy->outputPerceptrons[i]->m_synapses[j]->m_weight;
			
			synapse->m_weight = r;
			perceptron->addSynapse(synapse);
		}

		outputPerceptrons.push_back(perceptron);
	}
}
Brain::~Brain()
{
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