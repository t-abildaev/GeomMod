#pragma once

#include "Distribution.h"
#include <algorithm>

class Sample
{
protected:
	int size;
	int * sample;
	double p;
	int numOfObsStates;
public:
	Sample();
	Sample(int _size);
	virtual ~Sample() { delete[] sample; };
	virtual void simulate() = 0;
	void setProb(double _p) { p = _p; };
	double getProb() { return(p); };
	int getSize() { return(size); }
	int getNumOfObsStates() { return(numOfObsStates); }
	int * getSample() { return(sample); };
};

class Bernoulli : public Sample
{
public:
	Bernoulli(int _size) :
		Sample(_size) {};
	void simulate() override;
};

class Inverse : public Sample
{
public:
	Inverse(int _size) :
		Sample(_size) {};
	void simulate() override;
};