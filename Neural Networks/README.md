#Neural Network
	Neural Networks are the classic example of Machine Learning. Neural Networks are the form of machine learning most similar to animals’ organic learning, as the network of neurons is vaguely based on the neurons in our brains which are interconnected by synapses.
	The idea is that each neuron is activated based on the weights and biases connected to it combined with the activation outputs of the neurons from the previous layer. The network evolves and learns by adjusting these weights and biases to output the desired values.



	Conventionally, we group neurons into layers, which we can then say are connected to each other. Essentially, two connected layers means each neuron in the first layer is connected by a “synapses” with certain weights to each of the neurons in the following layer.

	Training a neural network is where the complications escalate. What is implemented here is a backpropagation, which is an algorithm that computes the gradients of the loss function according to the difference between the outputs from the network and the desired outputs, and uses this gradient to find the minimum of the loss function - which would be the “smartest” network, the network who’s weights and biases are as ideal as possible for the problem.
##Gradient Descent
	Gradient Descent is a way of computing which changes (“nudges”) need to be made to the weights of a neuron in order to minimize the cost of the network as efficiently as possible. Gradient Descent in effect calculates the gradient vector which shows how sensitive the output of the network is to changes to each of the neurons in the current layer (doing so recursively is backpropagation).
	We can visualize a two dimensional gradient descent, where the black line is our cost function, the blue dot is our weights and biases, and the red line is our gradient. What gradient descent tells us, is that if we follow the opposite of the gradient (-f’(x)) and move, as in adjust the weights and biases, proportionally to its size (represented by a longer red line below), we will reach a local (ideally global) minimum of the cost function in a very efficient manner.

We can prove that this will converge onto a local minimum:

Let’s call the cost function F(x) and assume it is defined and differentiable for all values.
Let ∇F be the derivative of F.
Assume point an is our current location on the cost function, representing the weights and biases of the network.
ϒ being our learning rate such that: ϒ ∈ ℝ+ resolves to the next point an+1 being:
an+1 = an - ϒ∇F(an)
and therefore we have a monotonic sequence such that:
F(a1) ≥ F(a2) ≥ F(a3) ≥ ... ≥ F(an)
meaning thatF(an) will be a local minimum of the cost function,
and as such, our weights and biases array - which is represented by an- will be semi-idealized to the problem
The neural network has learnt to “solve” the given problem.


##Backpropagation
	Backpropagation is one of the core factors of machine learning. Backpropagation in essence is a method of changing the weights and biases of layers in a neural network depending on the layer after them. This method is crucial in training a network, since when training we cannot directly change the outputs of the last layer to fit our problem, rather we change the weights and biases of all the neurons. Backpropagation allows us to quickly calculate how much to change the parameters of each neuron layer by layer simply by inputting our final output and our desired output.
	Backpropagation works by working backwards - from the last (output) layer, to the first layer - and calculating each time the effect that each neuron in that layer had on the output, and accordingly, should it be increased or decreased and by how much (this is done by calculating the error). Backpropagation then takes this calculation and uses it as the desired output for the previous layer, which in effect simplifies the network recursively so that we only ever update one layer as if it was the output layer and we knew exactly what outputs we wanted there.
##Utility
	The most alluring feature of neural networks is the fact that once they have been suited to the problem, they give predictions almost instantaneously. Contrary to KNN, which starts working with a solid accuracy immediately when launched, neural networks start off scoring poorly. Moreover, neural networks do not need an entire dataset to be loaded in memory in order to classify data; all they need is the weights and biases which best suit the problem - the weights and biases which produce the smallest cost function.
	This means that once we adequately train a neural network, we can save the weights and biases it calculated and import them at a later date and immediately have a high accuracy, while maintaining a short prediction time.
##Final Result

	Here we can see a proof of concept for this neural network. Obviously the performance is not ideal. However, this is simply since we are using a very small network - only two hidden layers with ten neurons each. Nevertheless, we clearly see that the network is improving, the iteration error, which is the output of our error function, is going down, and the validation performance is going up.
	The main limiting factor here is time; each training cycle takes approximately 12 minutes (disregard the time in the image, as a smaller training dataset was used to produce more results. Therefore, the actual error performance is also inaccurate). This is utterly impractical, especially since for our final product we want a more complex network with many more calculations. See the faster version of this network here.
##Code

```C++
#include "network.h"
#include "layer.h"
#include "DataHandler.h"
#include <numeric>
#include <algorithm>
#include <thread>
 
 
#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;
 
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
 
Network::Network(std::vector<int> spec, int inputSize, int numClasses, double learningRate)
{
    for (int i = 0; i < spec.size(); i++)
    {
        if (i == 0)
            layers.push_back(new Layer(inputSize, spec.at(i)));
        else
            layers.push_back(new Layer(layers.at(i - 1)->neurons.size(), spec.at(i)));
 
    }
    layers.push_back(new Layer(layers.at(layers.size() - 1)->neurons.size(), numClasses));
    this->learningRate = learningRate;
}
 
Network::~Network() {}
 
double Network::activate(std::vector<double> weights, std::vector<double> input)
{
    double activation = weights.back(); // bias term
    for (int i = 0; i < weights.size() - 1; i++)
    {
        activation += weights[i] * input[i];
    }
    return activation;
}
 
double Network::transfer(double activation)
{
    return 1.0 / (1.0 + exp(-activation));
}
 
double Network::transferDerivative(std::vector<double> errors, std::vector<double> inputs)
{
    double derivative = 0;
 
    for (int i = 0; i < errors.size(); i++) {
        derivative += errors[i] * inputs[i];
    }
 
    derivative *= 2.0 / errors.size();
 
    return derivative;
}
 
std::vector<double> Network::CalculateDerivative(Data * data, Layer * myLayer) {
 
    std::vector<double> errors1;
    std::vector<double> errors2;
 
    double gamma = 0.00000001;
 
    for (Neuron *n : myLayer->neurons) {
        double error = n->weights.at(n->weights.size() - 1); // bias
        for (int w = 0; w < n->weights.size() - 1; w++) {
            error += n->weights[w] * (data->getNormalizedFeatureVector()->at(w));
        }
        errors1.push_back(error);
    }
 
    for (Neuron *n : myLayer->neurons) {
        double error = n->weights.at(n->weights.size() - 1); // bias
        for (int w = 0; w < n->weights.size() - 1; w++) {
            error += n->weights[w] * (data->getNormalizedFeatureVector()->at(w) + gamma);
        }
        errors2.push_back(error);
    }
 
    std::vector<double> errors_difference;
 
    for (int i = 0; i < errors1.size(); i++) {
        errors_difference.push_back((errors2[i] - errors1[i]) / gamma);
    }
 
    return errors_difference;
}
std::vector<double> Network::CalculateDerivative(Layer * preLayer, Layer * myLayer) {
 
    std::vector<double> errors1;
    std::vector<double> errors2;
 
    double gamma = 0.00000001;
 
    for (Neuron *n : myLayer->neurons) {
        double error = n->weights.at(n->weights.size() - 1); // bias
        for (int w = 0; w < n->weights.size() - 1; w++) {
            error += n->weights[w] * preLayer->neurons[w]->output;
        }
        errors1.push_back(error);
    }
 
    for (Neuron *n : myLayer->neurons) {
        double error = n->weights.at(n->weights.size() - 1); // bias
        for (int w = 0; w < n->weights.size() - 1; w++) {
            error += n->weights[w] * (preLayer->neurons[w]->output + gamma);
        }
        errors2.push_back(error);
    }
 
    std::vector<double> errors_difference;
 
    for (int i = 0; i < errors1.size(); i++) {
        errors_difference.push_back((errors2[i] - errors1[i]) / gamma);
    }
 
    return errors_difference;
}
 
std::vector<double> Network::fprop(Data *data)
{
    std::vector<double> inputs = *data->getNormalizedFeatureVector();
    for (int i = 0; i < layers.size(); i++)
    {
        Layer *layer = layers.at(i);
        std::vector<double> newInputs;
        for (Neuron *n : layer->neurons)
        {
            double activation = this->activate(n->weights, inputs);
            n->output = this->transfer(activation);
            newInputs.push_back(n->output);
        }
        layer->layerOutputs = newInputs;
        inputs = newInputs;
    }
    return inputs; // output layer outputs
}
 
void Network::bprop(std::vector<double> deriv_errors) /// Backpropagation
{
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        Layer *layer = layers.at(i);
        std::vector<double> errors;
        if (i != layers.size() - 1)
        {
            for (int j = 0; j < layer->neurons.size(); j++)
            {
                double error = 0.0;
                for (Neuron *n : layers.at(i + 1)->neurons)
                {
                    error += (n->weights.at(j) * n->delta);
                }
                errors.push_back(error);
            }
        }
        else {
            // If is last layer (i == layers.size()-1)
            errors = deriv_errors;
        }
 
        for (int j = 0; j < layer->neurons.size(); j++)
        {
            Neuron *n = layer->neurons.at(j);
            n->delta = errors[j];
        }
    }
}
 
void Network::updateWeights(Data *data)
{
    std::vector<double> inputs = *data->getNormalizedFeatureVector();
    for (int i = 0; i < layers.size(); i++)
    {
        if (i != 0)
        {
            for (Neuron *n : layers.at(i - 1)->neurons)
            {
                inputs.push_back(n->output);
            }
        }
        for (Neuron *n : layers.at(i)->neurons)
        {
            for (int j = 0; j < inputs.size(); j++)
            {
                n->weights.at(j) += this->learningRate * n->delta * inputs.at(j);
            }
            n->weights.back() += this->learningRate * n->delta;
        }
        inputs.clear();
    }
}
 
int Network::predict(Data * data)
{
    std::vector<double> outputs = fprop(data);
    return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}
 
void Network::train(int numEpochs, int iter)
{
    for (int i = 0; i < numEpochs; i++)
    {
        double sumError = 0.0;
        std::vector<double> deriv_error;
 
        int output_vector_size = this->trainingData->at(0)->getClassVector().size();
        for (int i = 0; i < output_vector_size; i++) { deriv_error.push_back(0.0); }
 
        for (Data *data : *this->trainingData)
        {
            std::vector<double> outputs = fprop(data);
            std::vector<int> expected = data->getClassVector();
            double tempErrorSum = 0.0;
            for (int j = 0; j < outputs.size(); j++)
            {
                tempErrorSum += pow((double)expected.at(j) - outputs.at(j), 2);
                deriv_error[j] += (double)expected.at(j) - outputs.at(j);
            }
            sumError += tempErrorSum;
 
            for (int j = 0; j < output_vector_size; j++)
            {
                deriv_error.at(j) /= this->trainingData->size();
                deriv_error.at(j) *= 2;
            }
 
            bprop(deriv_error);
            updateWeights(data);
 
            for (int j = 0; j < output_vector_size; j++)
            {
                deriv_error.at(j) = 0;
            }
        }
        printf("Iteration: %d \t Error=%.4f\n", (iter * numEpochs) + i, sumError);
    }
}
 
double Network::test()
{
    double numCorrect = 0.0;
    double count = 0.0;
    for (Data *data : *this->testData)
    {
        count++;
        int index = predict(data);
        if (data->getClassVector().at(index) == 1) numCorrect++;
    }
 
    testPerformance = (numCorrect / count); // as decimal 0 -> 1
    return testPerformance;
}
 
void Network::validate()
{
    double numCorrect = 0.0;
    double count = 0.0;
    for (Data *data : *this->validationData)
    {
        count++;
        int index = predict(data);
        if (data->getClassVector().at(index) == 1) numCorrect++;
    }
    printf("Validation Performance: %.4f%%\n", (numCorrect*100) / count); // as percentage 0.0000% -> 100.0000%
}
 
int main()
{
    DataHandler *dataHandler = new DataHandler();
    dataHandler->readInputData("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-images.idx3-ubyte");
    dataHandler->readLabelData("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-labels.idx1-ubyte");
    dataHandler->countClasses();
    dataHandler->splitData();
    std::vector<int> hiddenLayers = { 10,10 };
    Network *netw = new Network(
        hiddenLayers,
        dataHandler->getTrainingData()->at(0)->getNormalizedFeatureVector()->size(),
        dataHandler->getClassCounts(),
        0.25);
    netw->setTrainingData(dataHandler->getTrainingData());
    netw->setTestData(dataHandler->getTestData());
    netw->setValidationData(dataHandler->getValidationData());
 
    printf("Initial Validation: \n");
    netw->validate();
    printf("Now training...\n");
 
    for (int i = 0; i < 100; i++) {
 
        TimeVar t1 = timeNow();
        netw->train(2, i); // This takes 674923436000 nanoseconds = 674.92 seconds = 11:15 minutes
        double trainTime = duration(timeNow() - t1);
 
        TimeVar t2 = timeNow();
        netw->validate();
        double validateTime = duration(timeNow() - t2);     
 
        printf("Training time: %.2f miliseconds\n", trainTime);
        printf("Validating time: %.2f miliseconds\n", validateTime);
 
        if (i % 10 == 0) {
            printf("Test Performance: %.3f\n", netw->test());
        }
    }
    printf("Test Performance: %.3f\n", netw->test());
 
    return 0;
}
```

##Improved Neural Network
	This version of the neural network is simply the previous version except with all the improvements listed in Evolvement - Practical Research. The main difference is that we used C code almost exclusively, instead of std::vector - which is a C++ class.
###Final Result

	As you can see, this version of the network reduced training time from ~674923436000 nanoseconds (674.92 seconds) to ~2661079700 nanoseconds (2.66 seconds); a 99.61% time reduction.
	This improvement means we can compile and train a more complex network with more data samples in significantly less time.
	Training the network for about an hour yielded a test performance of just over 80% with a simple network of one hidden layer with 32 neurons, which was plenty of proof to show that the network is in-fact learning. Thus, we conclude our proof-of-concept machine learning phase, and move to our actual goal.
###Code
```C++
data.h + data.cpp

#pragma once
 
#include <vector>
#include <stdint.h>
#include <stdio.h>
 
class data
{
private:
    std::vector<uint8_t> * feature_vector;
    std::vector<double> * normalized_feature_vector;
    double * normalized_feature_array;
    uint32_t normalized_feature_array_size;
    std::vector<int> * class_vector;
    int * class_array;
    int class_array_size;
    uint8_t label;
    uint8_t enumerated_label;
    double distance;
public:
    void set_distance(double distance);
    void set_feature_vector(std::vector<uint8_t> * vect);
    void set_normalized_feature_vector(std::vector<double> * norm_vect);
    void update_normalized_feature_array();
    void set_class_vector(int counts);
    void set_class_array(int counts);
    void append_to_feature_vector(uint8_t val);
    void append_to_feature_vector(double val);
    void set_label(uint8_t label);
    void set_enumerated_label(uint8_t label);
 
    double get_distance();
    int get_feature_vector_size();
    int get_feature_array_size();
    uint8_t get_label();
    uint8_t get_enumerated_label();
 
    std::vector<uint8_t> * get_feature_vector();
    std::vector<double> * get_normalized_feature_vector();
    double * get_normalized_feature_array();
    std::vector<int> * get_class_vector();
    int * get_class_array();
    int get_class_array_size();
 
    void c_only();
};
#include "data.h"
 
void data::set_distance(double distance) {
    this->distance = distance;
}
void data::set_feature_vector(std::vector<uint8_t> * vect) {
    this->feature_vector = vect;
}
void data::set_normalized_feature_vector(std::vector<double> * norm_vect) {
    this->normalized_feature_vector = norm_vect;
}
 
void data::set_class_vector(int counts) {
    this->class_vector = new std::vector<int>();
    for (int i = 0; i < counts; i++)
    {
        if (i == label)
            this->class_vector->push_back(1);
        else
            this->class_vector->push_back(0);
    }
}
void data::set_class_array(int counts) {
    if (class_array != NULL) { free(class_array); }
    else { class_array = (int*)calloc(counts, sizeof(int)); }
    
    class_array[label] = 1;
 
    class_array_size = counts;
}
 
void data::append_to_feature_vector(uint8_t val) {
    feature_vector->push_back(val);
}
void data::append_to_feature_vector(double val) {
    normalized_feature_vector->push_back(val);
}
void data::set_label(uint8_t label) {
    this->label = label;
}
void data::set_enumerated_label(uint8_t label) {
    this->enumerated_label = label;
}
 
double data::get_distance() {
    return this->distance;
}
 
int data::get_feature_vector_size() {
    return feature_vector->size();
}
uint8_t data::get_label() {
    return this->label;
}
uint8_t data::get_enumerated_label() {
    return this->enumerated_label;
}
 
std::vector<uint8_t> * data::get_feature_vector() {
    return this->feature_vector;
}
std::vector<double> * data::get_normalized_feature_vector() {
    return this->normalized_feature_vector;
}
std::vector<int> * data::get_class_vector() {
    return this->class_vector;
}
int * data::get_class_array() {
    return this->class_array;
}
 
double * data::get_normalized_feature_array() {
    return normalized_feature_array;
}
 
void data::update_normalized_feature_array() {
    if (normalized_feature_array != NULL) { free(normalized_feature_array); }
    normalized_feature_array_size = normalized_feature_vector->size();
    normalized_feature_array = (double *)malloc(normalized_feature_array_size * sizeof(double));
 
    for (int i = 0; i < normalized_feature_vector->size(); i++) {
        normalized_feature_array[i] = normalized_feature_vector->at(i);
    }
}
 
int data::get_feature_array_size() {
    return normalized_feature_array_size;
}
 
int data::get_class_array_size() {
    return class_array_size;
}
 
void data::c_only() {
    update_normalized_feature_array();
 
    if (class_array != NULL) { free(class_array); }
    class_array = (int *)malloc(class_vector->size() * sizeof(int));
    for (int i = 0; i < class_vector->size(); i++) {
        class_array[i] = class_vector->at(i);
    }
 
    delete normalized_feature_vector;
    delete feature_vector;
 
    delete class_vector;
}


data_handler.h + data_handler.cpp

#pragma once
 
#include "fstream"
#include "stdint.h"
#include "data.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <math.h>
#include <algorithm>
#include <random>
 
class data_handler
{
private:
    std::vector<data *> * data_vector; // all the data vectors
    std::vector<data *> * training_data;
    std::vector<data *> * validation_data;
    std::vector<data *> * test_data;
    
    int class_counts;
    int feature_vector_size;
    std::map<uint8_t, int> class_from_int;
    std::map<std::string, int> class_from_string;
 
    uint32_t max_data_vector_size = 60000;
 
public:
    const double TRAIN_SET_PERCENT = 0.70;
    const double TEST_SET_PERCENT = 0.20;
    const double VALID_SET_PERCENT = 0.10;
 
    data_handler();     // ctor
    ~data_handler();    // dtor
 
    void read_input_data_file(std::string file_path);
    void read_input_label_file(std::string file_path);
 
    void split_data();
    void count_classes();
    void normalize_data();
    void log();
 
    int get_class_count();
    int get_data_vector_size();
    int get_training_data_size();
    int get_test_data_size();
    int get_validation_data_size();
 
    std::vector<data *> * get_training_data();
    std::vector<data *> * get_validation_data();
    std::vector<data *> * get_test_data();
    std::map<uint8_t, int> get_class_map();
 
    uint32_t format(const unsigned char* bytes);
};
#include "data_handler.h"
 
data_handler::data_handler() {
    data_vector = new std::vector<data *>;
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}
data_handler::~data_handler() {
}
 
void data_handler::read_input_data_file(std::string file_path) {
    uint32_t magic = 0;
    uint32_t num_images = 0;
    uint32_t num_rows = 0;
    uint32_t num_cols = 0;
 
    unsigned char bytes[4];
    FILE *f;
    errno_t err = fopen_s(&f, file_path.c_str(), "rb");
    if (f)
    {
        int i = 0;
        while (i < 4)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                switch (i)
                {
                case 0:
                    magic = format(bytes);
                    i++;
                    break;
                case 1:
                    num_images = format(bytes);
                    i++;
                    break;
                case 2:
                    num_rows = format(bytes);
                    i++;
                    break;
                case 3:
                    num_cols = format(bytes);
                    i++;
                    break;
                }
            }
        }
        num_images = std::min(num_images, max_data_vector_size);
        printf("Done getting input data file header.\n");
 
        uint32_t image_size = num_rows * num_cols;
        for (i = 0; i < num_images; i++)
        {
            data * d = new data();
            d->set_feature_vector(new std::vector<uint8_t>());
            uint8_t element[1];
            for (int j = 0; j < image_size; j++)
            {
                if (fread(element, sizeof(element), 1, f))
                {
                    d->append_to_feature_vector(element[0]);
                }
            }
            this->data_vector->push_back(d);
            d->set_class_vector(class_counts);
        }
        normalize_data();
        feature_vector_size = data_vector->at(0)->get_feature_vector_size();
 
        printf("Successfully read %lu data entries.\n", data_vector->size());
        printf("The Feature Vector Size is: %d\n", feature_vector_size);
    }
    else
    {
        printf("Could not open or read input vectors file!\n");
        printf("Now exiting...\n");
        exit(1);
    }
}
void data_handler::read_input_label_file(std::string file_path) {
    uint32_t magic = 0;
    uint32_t num_images = 0;
    unsigned char bytes[4];
    FILE *f;
    errno_t err = fopen_s(&f, file_path.c_str(), "rb");
    if (f)
    {
        int i = 0;
        while (i < 2)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                switch (i)
                {
                case 0:
                    magic = format(bytes);
                    i++;
                    break;
                case 1:
                    num_images = format(bytes);
                    i++;
                    break;
                }
            }
        }
 
        printf("Done getting Label header.\n");
 
        num_images = std::min(num_images, max_data_vector_size);
 
        for (unsigned j = 0; j < num_images; j++)
        {
            uint8_t element[1];
            if (fread(element, sizeof(element), 1, f))
            {
                data_vector->at(j)->set_label(element[0]);
            }
        }
 
        printf("Successfully labeled %lu data entries.\n", num_images);
    }
    else
    {
        printf("Could not open or read input labels file!\n");
        printf("Now exiting...\n");
        exit(1);
    }
}
 
void data_handler::split_data() {
    std::unordered_set<int> used_indexes;
    int train_size = data_vector->size() * TRAIN_SET_PERCENT;
    int validation_size = data_vector->size() * VALID_SET_PERCENT;
    int test_size = data_vector->size() * TEST_SET_PERCENT;
 
    std::random_shuffle(data_vector->begin(), data_vector->end());
 
    // Training Data
    int count = 0;
    int index = 0;
    while (count < train_size)
    {
        training_data->push_back(data_vector->at(index++));
        count++;
    }
 
    // Validation Data
    count = 0;
    while (count < validation_size)
    {
        validation_data->push_back(data_vector->at(index++));
        count++;
    }
 
    // Test Data
    count = 0;
    while (count < test_size)
    {
        test_data->push_back(data_vector->at(index++));
        count++;
    }
 
    printf("Training Data Size: %lu.\n", training_data->size());
    printf("Validation Data Size: %lu.\n", validation_data->size());
    printf("Test Data Size: %lu.\n", test_data->size());
 
    if (training_data->size() + validation_data->size() + test_data->size() != data_vector->size()) {
        uint32_t amount_used = training_data->size() + validation_data->size() + test_data->size();
        printf("Only %.2f%% of vectors were used. %d were not sorted.\n", 100.0 * amount_used / data_vector->size(), data_vector->size() - amount_used);
    }
}
 
void data_handler::count_classes() {
    int count = 0;
    for (unsigned i = 0; i < data_vector->size(); i++)
    {
        if (class_from_int.find(data_vector->at(i)->get_label()) == class_from_int.end())
        {
            class_from_int[data_vector->at(i)->get_label()] = count;
            data_vector->at(i)->set_enumerated_label(count);
            count++;
        }
        else
        {
            data_vector->at(i)->set_enumerated_label(class_from_int[data_vector->at(i)->get_label()]);
        }
    }
 
    class_counts = count;
    for (data * data : *data_vector) {
        data->set_class_vector(class_counts);
        data->set_class_array(class_counts);
    }
 
    printf("Successfully Extracted %d Unique Classes.\n", class_counts);
}
void data_handler::normalize_data() {
    std::vector<double> mins, maxs;
    // fill min and max lists
 
    data * d = data_vector->at(0);
    for (uint8_t val : *(d->get_feature_vector()))
    {
        mins.push_back(val);
        maxs.push_back(val);
    }
 
    for (int i = 1; i < data_vector->size(); i++)
    {
        d = data_vector->at(i);
        for (int j = 0; j < d->get_feature_vector_size(); j++)
        {
            double value = (double)d->get_feature_vector()->at(j);
            if (value < mins.at(j)) mins[j] = value;
            if (value > maxs.at(j)) maxs[j] = value;
        }
    }
 
    // normalize data array
    for (int i = 0; i < data_vector->size(); i++)
    {
        data_vector->at(i)->set_normalized_feature_vector(new std::vector<double>());
        data_vector->at(i)->set_class_vector(class_counts);
        for (int j = 0; j < data_vector->at(i)->get_feature_vector_size(); j++)
        {
            // add normalized value to normalized_feature_vector
            if (maxs[j] - mins[j] == 0) 
            { data_vector->at(i)->append_to_feature_vector(0.0); }
            else {
                data_vector->at(i)->append_to_feature_vector((double)(data_vector->at(i)->get_feature_vector()->at(j) - mins[j]) / (maxs[j] - mins[j]));
            }
        }
    }
}
void data_handler::log() {
 
}
 
int data_handler::get_class_count() {
    return class_counts;
}
int data_handler::get_data_vector_size() {
    return data_vector->size();
}
int data_handler::get_training_data_size() { return training_data->size(); }
int data_handler::get_test_data_size() { return test_data->size(); }
int data_handler::get_validation_data_size() { return validation_data->size(); }
 
std::vector<data *> * data_handler::get_training_data() { return training_data; }
std::vector<data *> * data_handler::get_validation_data() { return validation_data; }
std::vector<data *> * data_handler::get_test_data() { return test_data; }
std::map<uint8_t, int> data_handler::get_class_map() { return class_from_int; }
 
uint32_t data_handler::format(const unsigned char* bytes)
{
    return (uint32_t)((bytes[0] << 24) |
        (bytes[1] << 16) |
        (bytes[2] << 8) |
        (bytes[3]));
}


common_data.h + common_data.cpp

#pragma once
 
#include "data.h"
#include <vector>
 
class common_data
{
protected:
    std::vector<data *> * training_data;
    std::vector<data *> * validation_data;
    std::vector<data *> * test_data;
public:
    void set_training_data(std::vector<data *> * vect);
    void set_validation_data(std::vector<data *> * vect);
    void set_test_data(std::vector<data *> * vect);
 
    std::vector<data *> * get_validation_data();
};


#include "common_data.h"
 
void common_data::set_training_data(std::vector<data *> * vect) {
    training_data = vect;
}
void common_data::set_validation_data(std::vector<data *> * vect) {
    validation_data = vect;
}
void common_data::set_test_data(std::vector<data *> * vect) {
    test_data = vect;
}
 
std::vector<data *> * common_data::get_validation_data() {
    return validation_data;
}


neuron.h + neuron.cpp

#pragma once
 
#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <time.h>
 
class neuron
{
public:
    double output;
    double delta;
    std::vector<double> weights;
    double * weights_array;
    uint32_t weights_array_size;
 
    neuron(int, int); // ctor
 
    void initialize_weights(int); // randomize weight values
 
    double activate(std::vector<double> inputs);
    double activate(int inputs_size, double * inputs); // c
 
    void c_only();
};
#include "neuron.h"
 
double generateRandomNumber(double min, double max)
{   
    double random = (double)rand() / RAND_MAX;
    return min + random * (max - min);
}
 
neuron::neuron(int prev_layer_size, int curr_layer_size)
{
    initialize_weights(prev_layer_size);
}
void neuron::initialize_weights(int prev_layer_size)
{
    for (int i = 0; i < prev_layer_size + 1; i++)
    {
        weights.push_back(generateRandomNumber(-1.0, 1.0));
    }
}
 
double neuron::activate(std::vector<double> inputs) {
    double activation = weights.back(); // bias
    for (int i = 0; i < weights.size() - 1; i++) {
        activation += weights[i] * inputs[i];
    }
    return activation;
}
double neuron::activate(int input_size, double * inputs) {
    double activation = weights_array[input_size]; // bias
    for (int i = 0; i < input_size; i++) {
        activation += weights_array[i] * inputs[i];
    }
    return activation;
}
 
void neuron::c_only() {
    if (weights.size() > 0) {
        weights_array = (double*)malloc(weights.size() * sizeof(double));
        weights_array_size = weights.size();
        for (int i = 0; i < weights.size(); i++) {
            weights_array[i] = weights.at(i);
        }
        weights.clear();
    }
}


layer.h + layer.cpp

#pragma once
 
#include "neuron.h"
#include <stdint.h>
#include <vector>
 
class layer
{
public:
    int current_layer_size;
    std::vector<neuron *> neurons;
    std::vector<double> layer_outputs;
    
    neuron ** neurons_array;
    int neurons_size;
    double * layer_outputs_array;
 
    layer(int prev_layer_size, int curr_layer_size); // ctor
 
    void c_only();
};
#include "layer.h"
 
layer::layer(int prev_layer_size, int curr_layer_size)
{
    for (int i = 0; i < curr_layer_size; i++)
    {
        neurons.push_back(new neuron(prev_layer_size, curr_layer_size));
    }
 
    this->current_layer_size = curr_layer_size;
}
 
void layer::c_only() {
    neurons_size = neurons.size();
 
    neurons_array = (neuron**)malloc(neurons_size * sizeof(neuron*));
 
    for (int i = 0; i < neurons_size; i++) {
        neurons_array[i] = neurons.at(i);
        neurons_array[i]->c_only();
    }
 
    layer_outputs_array = (double *)malloc(neurons_size * sizeof(double));
 
    neurons.clear();
    layer_outputs.clear();
}


network.h + network.cpp

#pragma once
 
#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "common_data.h"
#include "data_handler.h"
 
#include <numeric>
#include <algorithm>
#include <thread>
 
class network : public common_data
{
public:
    std::vector<layer *> layers;
    layer ** layers_array;
    int layers_size;
    double learning_rate;
    double test_performance;
 
    network(std::vector<int> hidden_layers_specification, int, int, double); // ctor
    ~network(); // dtor
 
    void c_only();
 
    std::vector<double> fprop(data * d); // forward propagation
    void bprop(std::vector<double> deriv_errors); // back propagation
    void update_weights(data * data); // update weights after bprop
 
    int fprop_c(data * d, double ** output); // forward propagation - c | returns class count
    void bprop_c(double * deriv_errors); // back propagation - c
    void update_weights_c(data * data); // update weights after bprop - c
 
    double train(); // returns error
    double validate(); // return percentage correct
    double test(); // return percentage correct
 
    double train_c(); // returns error - c
    double validate_c(); // return percentage correct - c
    double test_c(); // return percentage correct - c
 
    double transfer_activation(double activat);
 
    int predict(data * data);
    int predict_c(data * data);
 
};
#include "network.h"
 
 
network::network(std::vector<int> hidden_layers_specification, int input_size, int number_of_classes, double learning_rate) {
    for (int i = 0; i < hidden_layers_specification.size(); i++)
    {
        if (i == 0)
            layers.push_back(new layer(input_size, hidden_layers_specification.at(i)));
        else
            layers.push_back(new layer(layers.at(i - 1)->neurons.size(), hidden_layers_specification.at(i)));
 
    }
    layers.push_back(new layer(layers.at(layers.size() - 1)->neurons.size(), number_of_classes));
 
    this->learning_rate = learning_rate;
}
network::~network() {
 
}
std::vector<double> network::fprop(data * d) {
    std::vector<double> inputs = *d->get_normalized_feature_vector();
 
    for (int i = 0; i < layers.size(); i++)
    {
        layer * l = layers.at(i);
        std::vector<double> new_inputs;
        int neuron_index = 0;
        for (neuron * n : l->neurons)
        {
            double activation = n->activate(inputs);
            n->output = this->transfer_activation(activation);
            new_inputs.push_back(n->output);
        }
        l->layer_outputs = new_inputs;
        inputs = new_inputs;
    }
    return inputs; // output layer outputs
}
int network::fprop_c(data * d, double ** output) { // fprop with only c
    int inputs_size = d->get_feature_array_size();
    double * inputs = (double*)malloc(inputs_size * sizeof(double));
 
    memcpy(inputs, d->get_normalized_feature_array(), inputs_size * sizeof(double));
 
    for (int layer_index = 0; layer_index < layers_size; layer_index++) {
        layer * l = (layers_array[layer_index]);
        double * new_inputs = (double *)malloc(l->neurons_size * sizeof(double));
        for (int neuron_index = 0; neuron_index < l->neurons_size; neuron_index++) {
            neuron * n = (l->neurons_array[neuron_index]);
            double activation = n->activate(inputs_size, inputs);
            n->output = this->transfer_activation(activation);
            new_inputs[neuron_index] = n->output;
        }
        inputs_size = l->neurons_size;
        memcpy(l->layer_outputs_array, new_inputs, inputs_size * sizeof(double));
        inputs = (double *)realloc(inputs, inputs_size * sizeof(double));
        memcpy(inputs, new_inputs, inputs_size * sizeof(double));
        free(new_inputs);
    }
    *output = (double *)malloc(inputs_size * sizeof(double));
    memcpy(*output, inputs, inputs_size * sizeof(double));
    free(inputs);
 
    return inputs_size;
}
 
void network::bprop(std::vector<double> deriv_errors) {
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        layer * l = layers.at(i);
        std::vector<double> errors;
        if (i != layers.size() - 1)
        {
            for (int j = 0; j < l->neurons.size(); j++)
            {
                double error = 0.0;
                for (neuron *n : layers.at(i + 1)->neurons)
                {
                    error += (n->weights.at(j) * n->delta);
                }
                errors.push_back(error);
            }
        }
        else {
            // If is last layer (i == layers.size()-1)
            errors = deriv_errors;
        }
 
        for (int j = 0; j < l->neurons.size(); j++)
        {
            neuron * n = l->neurons.at(j);
            n->delta = errors[j];
        }
    }
}
void network::bprop_c(double * deriv_errors) {
    double * errors;
    for (int i = layers_size - 1; i >= 0; i--)
    {
        layer * l = (layers_array[i]);
        
        if (i != layers_size - 1)
        {
            errors = (double *)malloc(l->neurons_size * sizeof(double));
            for (int j = 0; j < l->neurons_size; j++)
            {
                /// for each neuron in current layer.
 
                double error = 0.0;
                for (int k = 0; k < layers_array[i + 1]->neurons_size; k++)
                {
                    /// for each neuron in next layer
                    neuron * n = (layers_array[i + 1]->neurons_array[k]);
                    error += (n->weights_array[j] * n->delta);
                }
                errors[j] = error;
            }
        }
        else {
            // If is last layer (i == layers.size()-1)
            errors = deriv_errors;
        }
 
        for (int j = 0; j < l->neurons_size; j++)
        {
            neuron * n = (l->neurons_array[j]);
            n->delta = errors[j];
        }
 
        if (deriv_errors != errors) {
            free(errors);
        }
    }
}
void network::update_weights(data * d) {
    std::vector<double> inputs = * d->get_normalized_feature_vector();
    for (int i = 0; i < layers.size(); i++)
    {
        if (i != 0)
        {
            for (neuron *n : layers.at(i - 1)->neurons)
            {
                inputs.push_back(n->output);
            }
        }
        for (neuron *n : layers.at(i)->neurons)
        {
            for (int j = 0; j < inputs.size(); j++)
            {
                n->weights.at(j) += this->learning_rate * n->delta * inputs.at(j);
            }
            n->weights.back() += this->learning_rate * n->delta;
        }
        inputs.clear();
    }
}
void network::update_weights_c(data * d) {
    int inputs_size = d->get_feature_array_size();
    double * inputs = (double *)malloc(inputs_size * sizeof(double));
    memcpy(inputs, d->get_normalized_feature_array(), inputs_size * sizeof(double));
 
    for (int i = 0; i < layers_size; i++)
    {
        if (i != 0)
        {
            inputs_size = layers_array[i - 1]->neurons_size;
            inputs = (double *)realloc(inputs, inputs_size * sizeof(double));
 
            for (int j = 0; j < layers_array[i - 1]->neurons_size; j++)
            {
                neuron * n = (layers_array[i-1]->neurons_array[j]);
                inputs[i] = n->output;
            }
        }
        for (int j = 0; j < layers_array[i]->neurons_size; j++)
        {
            neuron * n = (layers_array[i]->neurons_array[j]);
            for (int k = 0; k < inputs_size; k++)
            {
                n->weights_array[k] += this->learning_rate * n->delta * inputs[k];
            }
            n->weights_array[inputs_size] += this->learning_rate * n->delta;
        }
    }
 
    free(inputs);
}
 
double network::train() {
    double sum_error = 0.0;
    std::vector<double> deriv_error;
 
    int output_vector_size = this->training_data->at(0)->get_class_vector()->size();
    for (int i = 0; i < output_vector_size; i++) { deriv_error.push_back(0.0); }
 
    for (data * d : *this->training_data)
    {
        std::vector<double> outputs = fprop(d);
        std::vector<int> expected = *(d->get_class_vector());
        double tmp_sum_error = 0.0;
        for (int j = 0; j < outputs.size(); j++)
        {
            tmp_sum_error += pow((double)expected.at(j) - outputs.at(j), 2);
            deriv_error[j] += (double)expected.at(j) - outputs.at(j);
        }
        sum_error += tmp_sum_error;
 
        for (int j = 0; j < output_vector_size; j++)
        {
            deriv_error.at(j) /= this->training_data->size();
            deriv_error.at(j) *= 2;
        }
 
        bprop(deriv_error);
        update_weights(d);
 
        for (int j = 0; j < output_vector_size; j++)
        {
            deriv_error.at(j) = 0;
        }
    }
 
    return sum_error;
}
double network::validate() {
    double num_correct = 0.0;
    double count = 0.0;
    for (data * d : *this->validation_data)
    {
        count++;
        int index = predict(d);
        if (d->get_class_vector()->at(index) == 1) { num_correct++; }
    }
 
    return num_correct / count;
}
double network::test() {
    double num_correct = 0.0;
    double count = 0.0;
    for (data * d : *this->test_data)
    {
        count++;
        int index = predict(d);
        if (d->get_class_vector()->at(index) == 1) { num_correct++; }
    }
 
    return num_correct / count;
}
 
double network::train_c() {
    double sum_error = 0.0;
    int output_vector_size = this->training_data->at(0)->get_class_array_size();
    double * deriv_error = (double *)calloc(output_vector_size, sizeof(double));
 
    for (data * d : *this->training_data)
    {
        double * outputs;
        fprop_c(d, &outputs);
 
        int * expected = d->get_class_array();
        double tmp_sum_error = 0.0;
        for (int j = 0; j < output_vector_size; j++)
        {
            double exp = (double)expected[j];
            double out = outputs[j];
            tmp_sum_error += pow(exp - out, 2);
            deriv_error[j] += exp - out;
        }
        sum_error += tmp_sum_error;
 
        for (int j = 0; j < output_vector_size; j++)
        {
            deriv_error[j] /= this->training_data->size();
            deriv_error[j] *= 2;
        }
 
        bprop_c(deriv_error);
        update_weights_c(d);
 
        for (int j = 0; j < output_vector_size; j++)
        {
            deriv_error[j] = 0;
        }
 
        free(outputs);
    }
 
    free(deriv_error);
 
    return sum_error;
}
double network::validate_c() {
    double num_correct = 0.0;
    double count = 0.0;
    for (data * d : *this->validation_data)
    {
        count++;
        int index = predict_c(d);
        if (d->get_class_array()[index] == 1) { num_correct++; }
    }
 
    return num_correct / count;
}
double network::test_c() {
    double num_correct = 0.0;
    double count = 0.0;
    for (data * d : *this->test_data)
    {
        count++;
        int index = predict_c(d);
        if (d->get_class_array()[index] == 1) { num_correct++; }
    }
 
    return num_correct / count;
}
 
double network::transfer_activation(double activat) {
    return 1.0 / (1.0 + exp(-activat));
}
 
int network::predict(data * data) {
    std::vector<double> outputs = fprop(data);
    return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}
int network::predict_c(data * data) {
    double * outputs;
    int class_count = fprop_c(data, &outputs);
 
    int max_ind = 0;
    double max = 0;
    for (int i = class_count - 1; i >= 0; i--) {
        if (outputs[i] > max) { max_ind = i; max = outputs[i]; }
    }
 
    free(outputs);
 
    return max_ind;
}
 
void network::c_only() {
    layers_size = layers.size();
    layers_array = (layer **)malloc(layers_size * sizeof(layer *));
    for (int i = 0; i < layers_size; i++) {
        layers_array[i] = (layers.at(i));
    }
 
    layers.clear();
 
    for (int i = 0; i < training_data->size(); i++) {
        training_data->at(i)->c_only();
    }
    for (int i = 0; i < validation_data->size(); i++) {
        validation_data->at(i)->c_only();
    }
    for (int i = 0; i < test_data->size(); i++) {
        test_data->at(i)->c_only();
    }
 
    for (int i = 0; i < layers_size; i++) {
        layer * l = layers_array[i];
        l->c_only();
    }
 
    printf("Network is now C only!\n");
}


main.cpp

#include <iostream>
 
#include "network.h"
 
#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;
 
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
 
int main()
{
    srand(time(NULL));
 
    data_handler * _data_handler = new data_handler();
    _data_handler->read_input_data_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-images.idx3-ubyte");
    _data_handler->read_input_label_file("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-labels.idx1-ubyte");
    _data_handler->count_classes();
    _data_handler->split_data();
 
    std::vector<int> hidden_layers = { 10,10 };
    network * netw = new network(
        hidden_layers,
        _data_handler->get_training_data()->at(0)->get_feature_vector_size(),
        _data_handler->get_class_count(),
        0.25);
 
    netw->set_training_data(_data_handler->get_training_data());
    netw->set_validation_data(_data_handler->get_validation_data());
    netw->set_test_data(_data_handler->get_test_data());
 
 
    printf("Initial Validation: \n");
    printf("Validation performance: %.4f%%\n", 100.0 * netw->validate());
 
    netw->c_only();
 
    printf("Initial Validation - C: \n");
    printf("Validation performance: %.4f%%\n", 100.0 * netw->validate_c());
    printf("Now training...\n");
 
    
 
    for (int i = 0; i < 100000; i++) {
 
        TimeVar t1 = timeNow();
        printf("Training error @ iteration %d: %.4f\n", i, netw->train_c());
        double train_time = duration(timeNow() - t1);
 
        printf("Training time: %.2f nanoseconds\n", train_time);
        
        if (i % 5 == 0) {
            TimeVar t2 = timeNow();
            printf("Validation performance: %.4f%%\n", 100.0 * netw->validate_c());
            double validate_time = duration(timeNow() - t2);
            printf("Validating time: %.2f nanoseconds\n", validate_time);
        }
 
 
        if (i % 25 == 0) {
            printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());
        }
    }
    printf("Test Performance: %.4f%%\n", 100.0 * netw->test_c());
 
    return 0;
}


```
