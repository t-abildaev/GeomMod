#include <iostream>
#include <cmath>
#include "pch.h"
#include "Distribution.h"

Distribution::Distribution() :
	p(0.5), numOfStates(ceil(log(eps) / log(0.5)))
{
	density = new double[numOfStates];
	calculateDensity();
}

void Distribution::setProb(double _p)
{
	delete[] density;
	p = _p;
	numOfStates = ceil(log(eps) / log(1 - p));
	density = new double[numOfStates] {};
	calculateDensity();
}

void Distribution::calculateDensity()
{
	delete[] density;
	density = new double[numOfStates] {};
	double s = 0;
	double q = 1 - p;
	density[0] = p;
	s += density[0];
	for (int i = 1; i < numOfStates - 1; ++i)
	{
		density[i] = density[i - 1] * q;
		s += density[i];
	}
	density[numOfStates - 1] = 1 - s;
}