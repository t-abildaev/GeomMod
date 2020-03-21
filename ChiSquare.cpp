#include "pch.h"
#include "ChiSquare.h"

ChiSquare::ChiSquare()
	: numOfTotalStates(0), expFrequency(0), obsFrequency(0),
	chiSquare(0), pValue(0), degOfFreedom(0) {}

void ChiSquare::set(Distribution & dist, Sample * sample)
{
	int numOfStates = dist.getNumOfStates();
	int numOfObsStates = sample->getNumOfObsStates();
	int size = sample->getSize();
	numOfTotalStates = max(numOfStates, numOfObsStates);
	delete[] expFrequency;
	delete[] obsFrequency;
	expFrequency = new double[numOfTotalStates] {};
	obsFrequency = new double[numOfTotalStates] {};
	for (int i = 0; i < numOfStates; ++i)
		expFrequency[i] = dist.getDensity()[i] * size;
	for (int i = 0; i < size; ++i)
		++obsFrequency[sample->getSample()[i]];
	//for (int i = 0; i < numOfObsStates; ++i)
		//obsFrequency[i] = std::count(sample->getSample(), sample->getSample() + size, i);
}

void ChiSquare::calculate()
{
	double * cumExpFrequency = new double[numOfTotalStates] {};
	double * cumObsFrequency = new double[numOfTotalStates] {};
	double * diffOfFrequencies = new double[numOfTotalStates] {};
	cumExpFrequency[0] = expFrequency[0];
	cumObsFrequency[0] = obsFrequency[0];
	diffOfFrequencies[0] = obsFrequency[0] - expFrequency[0];
	for (int i = 1; i < numOfTotalStates; ++i)
	{
		cumExpFrequency[i] = cumExpFrequency[i - 1] + expFrequency[i];
		cumObsFrequency[i] = cumObsFrequency[i - 1] + obsFrequency[i];
		diffOfFrequencies[i] = obsFrequency[i] - expFrequency[i];
	}
	degOfFreedom = group(cumExpFrequency, cumObsFrequency, diffOfFrequencies);
	chiSquare = 0;
	for (int i = 0; i < degOfFreedom + 1; ++i)
		chiSquare += diffOfFrequencies[i] * diffOfFrequencies[i] / cumExpFrequency[i];
	CHI(1, degOfFreedom, chiSquare, pValue);
	delete[] cumExpFrequency;
	delete[] cumObsFrequency;
	delete[] diffOfFrequencies;
}

int ChiSquare::group(double * cumExpFrequency, double * cumObsFrequency, double * diffOfFrequencies)
{
	int count = 0;
	int j = 0;
	while (j < numOfTotalStates)
	{
		while (j < numOfTotalStates && cumExpFrequency[j] < C) ++j;
		for (int i = j + 1; i < numOfTotalStates; ++i)
		{
			cumExpFrequency[i] -= cumExpFrequency[j];
			cumObsFrequency[i] -= cumObsFrequency[j];
		}
		if (j >= numOfTotalStates) break;
		cumObsFrequency[count] = cumObsFrequency[j];
		cumExpFrequency[count] = cumExpFrequency[j];
		diffOfFrequencies[count] = cumObsFrequency[count] - cumExpFrequency[count];
		++count;
		++j;
	}
	if (cumExpFrequency[numOfTotalStates - 1] > 0)
	{
		cumExpFrequency[count - 1] += cumExpFrequency[numOfTotalStates - 1];
		cumObsFrequency[count - 1] += cumObsFrequency[numOfTotalStates - 1];
		diffOfFrequencies[count - 1] = cumObsFrequency[count - 1] - cumExpFrequency[count - 1];
	}
	return(count - 1);
}