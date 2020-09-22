#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "stdint.h"
#include <iostream>

class dataChunk
{
public:
	dataChunk();
	~dataChunk();

	char ckID[4];
	char ckSize[4];

	void AppendData(char * dat);
	std::vector<char *> * getData();

	int getSize();

	void SetID(char *);

private:
	std::vector<char *> * data;
};

