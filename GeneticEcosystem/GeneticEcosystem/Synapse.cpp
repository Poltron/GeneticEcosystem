#include "stdafx.h"
#include "Synapse.h"
#include "Settings.h"
#include "Helpers.h"

Synapse::Synapse()
{
	m_weight = Helpers::randomRangeFloat(SYNAPSE_WEIGHT_MIN, SYNAPSE_WEIGHT_MAX);
	
	if (Helpers::randomRangeFloat(0, 1) < 0.3f)
		m_index = Helpers::randomRangeInt(0, BRAIN_INPUT_LAYER_SIZE - 1);
	else
		m_index = Helpers::randomRangeInt(0, BRAIN_SIZE - 1);
}

Synapse::Synapse(float weight, int index)
{
	m_weight = weight;
	m_index = index;
}

Synapse::~Synapse()
{
}
