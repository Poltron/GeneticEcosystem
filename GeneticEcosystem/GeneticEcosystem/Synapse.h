#pragma once
class Perceptron;

class Synapse
{
public:
	float m_weight; 
	int m_index;

	Synapse();
	Synapse(float weight, int index);
	~Synapse();
};
