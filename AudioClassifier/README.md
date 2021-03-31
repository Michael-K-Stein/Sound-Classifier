# Multifunction Audio Classifier
	After some tinkering with the Music Genre Classifier, I realized there was more potential than I had anticipated; the exact same code and structure could be used to classify any type of sound! The entire project was written to be as generic and modular as possible, and therefore was easily adaptable to a few more problems.
	
## Network Output Files - .net
The only real difference between the various solutions to the problems is the resulting network file (.net file, explained below), which is produced uniquely for each problem and its dataset. This network file actually turned out to be invaluable as during testing, some versions of the network required multiple days of training. Unfortunately, the power lines in Tel Aviv are extremely volatile during winter (around when I began training the networks), and the power would shut off every few hours. This meant I would lose hours of work each time the computer crashed. To overcome this, the network export files were designed. Network export files contain all the information needed to recreate a “carbon” copy of a trained network.
	A Network Output file contains the weights and biases of each and every neuron, and information needed to redesign the structure (layer layout) of the network. Additionally, to improve the ability to sort between network output files, before each export the network tests itself and appends its best score into the output file.
## Final Result:
![image28](https://user-images.githubusercontent.com/36902556/113140566-42ff1c00-9231-11eb-97b5-d567c318e3ed.png)
![image12](https://user-images.githubusercontent.com/36902556/113140607-50b4a180-9231-11eb-861b-0a1c3ccadbe8.png)

Using this Audio Classifier, we are able to classify:
Different genres of music:
The first test was to see if we can differentiate between genres of music. The result was that we can classify with a little over 78% accuracy ‘Classical Music’, ‘Electronic Dance Music’, ‘Jazz’, and ‘Rock’. This test was done with a network of 4000 -> 420 -> 32 -> 4, and took around 11 training iterations on a dataset of 23,707 feature vectors.
Network File: MNIST_Network_Output3.net
Different people speaking:
The next test was to differentiate between people’s voices. As explained in the Theoretical Background, this is almost identical to our previous problem. Hence, we used the exact same program, with the exact same network configuration to give us an accuracy of over 96% when differentiating between ‘Angela Merkel’, ‘Martin Luther King Jr’, ‘John F Kennedy’, and ‘Bucky Roberts’, after around 30 training cycles of 2,838 data samples.
Network File: FourV_96.net
Different instruments:
Now let’s try instruments! Testing with these following classifications and files Piano, Guitar, Violin, returned a test accuracy of ~98% on a very simple network of 4000 -> 30 -> 3 with 7547 feature vectors.
Network file: Instr98.net
