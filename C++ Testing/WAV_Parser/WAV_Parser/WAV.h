#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "stdint.h"
#include <iostream>
#include "dataChunk.h"

class WAV
{
public:
	WAV();
	~WAV();

	void Load(std::string filePath);
	void Parse();

	char ckID[5];
	char ckSize[5];
	char waveid[5];

	std::vector<char> * data;
	std::vector<dataChunk *> * chunks;
};

