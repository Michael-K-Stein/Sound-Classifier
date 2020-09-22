#ifndef __DATA_H
#define __DATA_H

#include "stdint.h"
#include "stdio.h"
#include <vector>


#pragma once
class data
{
	std::vector<double> * normalizedFeatureVector;
	std::vector<double> * desiredOutput;
	double distance;

public:
	data();
	~data();

	void setNormalizedFeatureVector(std::vector<double>*);
	void setDesiredOutput(std::vector<double>*);
	void set_distance(double);

	double get_distance();
	std::vector<double> * getNormalizedFeatureVector();
	std::vector<double> * getDesiredOutput();
	int getLabel();

};

#endif