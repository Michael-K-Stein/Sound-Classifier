#pragma once

//#ifndef __CommandLineFunctions
//#define __CommandLineFunctions


#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <map>

#include "AudioFile.h"
#include "FFT.h"

#include "Neural_Network/network.h"

/// Globals

// Reference to global neural network
static Network * network;

// Reference to global dataset
static Data_Handler * data_handler;


// Global strings (character arrays)
static char DataSetLabel[128] = { 0 };
static char OutputPath[128] = { 0 };
static char fn_label_map[128];
static char fn_data_map[128];
static char fn_vectors[128];
static char fn_labels[128];


/// Helping_Funcs
std::string get_current_dir();
inline bool file_exists(const std::string& name);
int getFileSize(FILE * f); // This will move the file pointer to SEEK_SET + 0
uint32_t convert_to_big_endian(uint32_t b);
uint32_t convert_to_little_endian(uint32_t b);
// Change the last character of a string into a null byte (if it's a newline).
void fix_fgets(char * inp);


// Check if an input matches a command
bool cmpcommand(char * inp, const char * command);

// Count the amount of arguments given in an input command (as ' ' delimiter)
int CountCommandArgs(char * command);

// Get command input argument at index as place it into output (also allocate memory for output)
int GetCommandArg(char * command, int index, char ** output);

/// Command_Line_Commands

// Update file paths using the user specified outputpath and the dataset global name
int UpdateFilePaths();

// Change the path of the output directory
int ChangeOutputPath();

// Change the dataset's name
int ChangeDataSetName();

// Count how many labels exist in the label map file.
int getExistingLabelsCount(uint8_t * existing_labels, FILE * fLabelMap);

// Proccess 'list' commands
int ListView(char * command);

// Prints the classes and their labels, which are stored in the label map file.
int ViewClassifications();

// Prints the file paths of each file in the dataset sorted by their labels.
int ViewDataFiles();

// Add an item to a dataset
int AddDataToSet();

// Initialize a new dataset
int Initialize();

// predict the label of a file using the Classifier
/*
	The predict function can take arguments such as:
	1) a file path.
		The first input can be the relative path to a vectors file which you want to use instead of processing a wav file into a vectors file.
	2) the '-f' argument.
		This is passed into the classifier as 'fast' which tells the program to only check some of the data points (arbitrarily) and not all of them.
		This obviously results in faster execution time (10x faster - since we take only a tenth of the vectors), but there is a penalty to the accuracy of the results. Though the final prediction is still usually correct.
*/
int Predict(char * command);

// Initialize Network of size / model
int InitNetwork(char * command);

// Load an existing network
int LoadNetwork(char * command);

// Export the loaded network
int ExportNetwork(char * command);

// Train the loaded network
int TrainNetwork(char * command);

// Validate the loaded network
int ValidateNetwork();

// Train and Export the network
int AFKTrain();

// Load dataset into data_handler
int LoadDataHandler();

// Load predict file
int LoadPredictFile(char * command);

// Exit
int ExitProgram();

enum HelpMenus {
	NoHelpMenu,
	GeneralHelpMenu,
	DataSetHelpMenu,
	PreidctionHelpMenu,
	NetworkHelpMenu,
	AllHelpMenu
};

// Show help menu
int Help(HelpMenus helpMenu);
int Help(char * command);


// Gets user input and compares it to valid commands
int ProccessCommands();


//#endif // !__CommandLineFunctions