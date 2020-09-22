#include "dataChunk.h"



dataChunk::dataChunk()
{
	data = new std::vector<char *>;
}


dataChunk::~dataChunk()
{
}

void dataChunk::AppendData(char * dat) {
	data->push_back(dat);
}

int dataChunk::getSize() {
	return data->size();
}

std::vector<char *> * dataChunk::getData() {
	std::vector<char *> * tmpData = new std::vector<char *>;
	for (int i = 0; i < getSize(); i++) {
		tmpData->push_back(data->at(i));
	}
	if (getSize() % 2 == 1) {
		tmpData->push_back('\0');
	}

	return tmpData;
}

void dataChunk::SetID(char * c) {
	ckID[0] = c[0];
	ckID[1] = c[1];
	ckID[2] = c[2];
	ckID[3] = c[3];
}