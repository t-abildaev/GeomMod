#include <chrono>
#include "pch.h"
#include "Sample.h"
#include "Random.h"

Sample::Sample() :
	size(-100), sample(0), p(0.5)
{
	Distribution dist;
	dist.setProb(p);
	numOfObsStates = dist.getNumOfStates();
}

Sample::Sample(int _size) :
	size(_size), p(0.5)
{
	sample = new int[size] {};
	Distribution dist;
	dist.setProb(0.5);
	numOfObsStates = dist.getNumOfStates();
}

void Bernoulli::simulate()
{
	int j = 0;
	for (int i = 0; i < size; ++i)
	{
		while (mtunif() > p) ++j;
		sample[i] = j;
		j = 0;
	}
	numOfObsStates = *(std::max_element(sample, sample + size)) + 1;
}

void Inverse::simulate()
{
	int j = 0;
	double rand = 0;
	double _p = p;
	double q = 1 - p;
	double sum = 0;
	for (int i = 0; i < size; ++i)
	{
		rand = mtunif();
		sum = p;
		while (rand > sum) { _p *= q; sum += _p; ++j; }
		sample[i] = j;
		_p = p;
		j = 0;
	}
	numOfObsStates = *(std::max_element(sample, sample + size)) + 1;
}