#ifndef __ML

#pragma once

#include "Network/network.h"
#include "../KNN Files/knn.h"

/* Input in the form:

[
Blocked infront,
Blocked on left,
Blocked on right,
Head pos X,
Head pos Y,
Fruit pos X,
Fruit pos Y,
Current Direction
]

*/

/* Output in the form:

[
Continue straight,
Turn Left,
Turn Right
]

*/

class ML {

public:
	ML();
	~ML();

	int Decide(data * data);

	void Update(data * d, bool good);

	void Train();

	void AddTrainingData(data * d);

private:
	Network * net;
	knn * KNN;
	std::vector<double> LastOut;


	bool printing = false;
};

#endif