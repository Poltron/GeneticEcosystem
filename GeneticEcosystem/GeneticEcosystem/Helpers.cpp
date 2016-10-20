#include "stdafx.h"
#include "Helpers.h"
#include <random>


float Helpers::randomRangeFloat(float min, float max)
{
	float range = max - min;
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / range);
	return (r + min);
}

int Helpers::randomRangeInt(int min, int max)
{
	return (min + (rand() % (int)(max - min + 1)));
}

float Helpers::cap(float f)
{
	if (f < -1)
		return -1;
	else if (f > 1)
		return 1;

	return f;
}