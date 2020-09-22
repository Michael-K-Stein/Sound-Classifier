#include "data.h"

data::data()
{
	normalizedFeatureVector = new std::vector<double>;
}


data::~data()
{
}

std::vector<double> * data::getNormalizedFeatureVector()
{
	return normalizedFeatureVector;
}
void data::setNormalizedFeatureVector(std::vector<double>* vect)
{
	normalizedFeatureVector = vect;
}
std::vector<double> * data::getDesiredOutput()
{
	return desiredOutput;
}
void data::setDesiredOutput(std::vector<double>* vect)
{
	desiredOutput = vect;
}

void data::set_distance(double val) {
	distance = val;
}


double data::get_distance() {
	return distance;
}

int data::getLabel() {
	double minVal = desiredOutput->at(0);
	int minInd = 0;

	for (int i = 1; i < desiredOutput->size(); i++) {
		if (desiredOutput->at(i) < minVal) {
			minVal = desiredOutput->at(i);
			minInd = i;
		}
	}

	return minInd;
}