#include "ML.h"

ML::ML(){
	//std::vector<int> hiddenLayers = { 2 };
	std::vector<int> hiddenLayers = { 5, 5 };
	net = new Network(hiddenLayers, /* Input Vector Size */ 8, /* Output Vector Size */ 3);

	KNN = new knn(3);

	std::vector<data *> * trainingData = new std::vector<data *>;
	KNN->set_training_Data(trainingData);
}
ML::~ML(){}

int ML::Decide(data * data) {
	LastOut = net->Proccess(data);
	int minIndex = 0;
	double minValue = LastOut.at(0);
	for (int i = 1; i < LastOut.size(); i++)
	{
		if (LastOut.at(i) < minValue)
		{
			minValue = LastOut.at(i);
			minIndex = i;
		}
	}

	KNN->find_knearest(data);
	int prediction = KNN->predict();

	minIndex = prediction;

	if (printing) {
		if (minIndex == 0) {
			printf("0 | Decided To Continue Straight\n");
		}
		else if (minIndex == 1) {
			printf("1 | Decided To Go Left\n");
		}
		else if (minIndex == 2) {
			printf("2 | Decided To Go Right\n");
		}
	}


	return minIndex;
}

void ML::Update(data * d, bool good) {
	if (printing) {
		if (good) {
			printf("true | That was a good idea\n");
		}
		else {
			printf("false | That was a bad idea\n");
		}
	}
	if (good) {
		net->bprop(d, &LastOut);
		net->updateWeights(d);
	}
	else {
		std::vector<double> out = LastOut;

		for (int i = 0; i < out.size(); i++) {
			out.at(i) = 1 - out.at(i);
		}

		net->bprop(d, &out);
		net->updateWeights(d);
	}
}

bool RandomChance(int something, int inSomething) { // Example: RandomChance(1, 5) will return true 1/5 = 20% of the time
	int r = rand() % inSomething;
	return (r < something);
}

void ML::Train() {

	std::vector<data *> * trainingData = new std::vector<data *>;

	// Generate 100 example cases
	for (int i = 0; i < 1000; i++) {
		std::vector<double> * dat = new std::vector<double>;
		srand(i);
		dat->push_back(RandomChance(1, 15));
		dat->push_back(RandomChance(1, 15));
		dat->push_back(RandomChance(1, 15));

		dat->push_back(rand() % 20);
		dat->push_back(rand() % 20);
		dat->push_back(rand() % 20);
		dat->push_back(rand() % 20);
		dat->push_back(rand() % 4);

		std::vector<double> * des = new std::vector<double>;

		if (dat->at(0) && dat->at(1)) { // If snake is blocked
			des->push_back(0);
			des->push_back(0);
			des->push_back(1);
		}
		else if (dat->at(1) && dat->at(2)) {
			des->push_back(1);
			des->push_back(0);
			des->push_back(0);
		}
		else if (dat->at(0) && dat->at(2)) {
			des->push_back(0);
			des->push_back(1);
			des->push_back(0);
		}
		else // If is not blocked, go depending on angle to fruit
		{
			double deltaX = dat->at(3) - dat->at(5);
			double deltaY = dat->at(4) - dat->at(6);

			double wX = deltaX / (deltaX + deltaY);
			double wY = deltaY / (deltaX + deltaY);

			int dir = dat->at(7); // 0 = up. Go clockwise


			double distIfStraight;
			double distIfLeft;
			double distIfRight;

			double dX = deltaX;
			double dY = deltaY;

			if (dir == 0) {
				dY++;
			}
			else if (dir == 1) {
				dX++;
			}
			else if (dir == 2) {
				dY--;
			}
			else if (dir == 3) {
				dX++;
			}
			distIfStraight = pow(pow(dX, 2) + pow(dY, 2), 0.5);
			
			dX = deltaX;
			dY = deltaY;
			if (dir == 0) {
				dX--;
			}
			else if (dir == 1) {
				dY++;
			}
			else if (dir == 2) {
				dX++;
			}
			else if (dir == 3) {
				dY--;
			}
			distIfRight = pow(pow(dX, 2) + pow(dY, 2), 0.5);

			dX = deltaX;
			dY = deltaY;
			if (dir == 0) {
				dX++;
			}
			else if (dir == 1) {
				dY--;
			}
			else if (dir == 2) {
				dX--;
			}
			else if (dir == 3) {
				dY++;
			}
			distIfLeft = pow(pow(dX, 2) + pow(dY, 2), 0.5);


			double totDist = distIfStraight + distIfLeft + distIfRight;

			des->push_back(1-(distIfStraight / totDist));
			des->push_back(1-(distIfLeft / totDist));
			des->push_back(1-(distIfRight / totDist));


			/*if (deltaY < 0 && dir == 0) { des->push_back(wY); }
			else if (deltaY > 0 && dir == 2) { des->push_back(wY); }
			else if (deltaY > 0 && dir == 0) { des->push_back(-wY); }
			else if (deltaY < 0 && dir == 2) { des->push_back(-wY); }*/

		}

		data * d = new data();
		d->setNormalizedFeatureVector(dat);
		d->setDesiredOutput(des);

		trainingData->push_back(d);
	}

	/*std::vector<double> * dat = new std::vector<double>;
	dat->push_back(0);
	dat->push_back(0);
	dat->push_back(0);

	dat->push_back(5);
	dat->push_back(2);
	dat->push_back(5);
	dat->push_back(4);
	dat->push_back(0);

	std::vector<double> * des = new std::vector<double>;
	des->push_back(0);
	des->push_back(0.5);
	des->push_back(0.5);

	data * d = new data();
	d->setNormalizedFeatureVector(dat);
	d->setDesiredOutput(des);*/

	/*for (int i = 0; i < 100; i++) {
		trainingData->push_back(d);
	}*/

	KNN->set_training_Data(trainingData);

	for (data * d : *trainingData)
	{
		net->fprop(d);
		net->bprop(d, d->getDesiredOutput());
		net->updateWeights(d);
	}

	//int a = Decide(d);
	//int b = a;
}

void ML::AddTrainingData(data * d) {
	KNN->append_training_data(d);
}