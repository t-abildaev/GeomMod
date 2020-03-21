#pragma once
#include "probdist.h"
#include "Sample.h"

class ChiSquare
{
private:
	const int C = 5;
	int numOfTotalStates;
	int numOfRect;
	double * expFrequency{};
	double * obsFrequency{};
	double chiSquare{};
	double pValue{};
	int degOfFreedom{};
public:
	ChiSquare();
	~ChiSquare() { delete[] expFrequency; delete[] obsFrequency; };
	void set(Distribution & dist, Sample * sample);
	int getNumOfTotalStates() { return(numOfTotalStates); };
	double getChiSquare() { return(chiSquare); };
	double getPValue() { return(pValue); };
	double * getExpFrequency() { return(expFrequency); };
	double * getObsFrequency() { return(obsFrequency); };
	int getDegOfFreedom() { return(degOfFreedom); };
	void calculate();
	int group(double * cumExpFrequency, double * cumObsFrequency, double * diffOfFrequencies);
	double maximum() { return(max(*(std::max_element(expFrequency, expFrequency + numOfTotalStates)), *(std::max_element(obsFrequency, obsFrequency + numOfTotalStates)))); };
};