#pragma once

class Distribution
{
	const double eps = 1e-3;
	double p;
	int numOfStates;
	double * density;
public:
	Distribution();
	~Distribution() { delete[] density; };
	void setProb(double _p);
	double getProb() { return(p); }
	void calculateDensity();
	double * getDensity() { return(density); };
	int getNumOfStates() { return(numOfStates); }
};