#include "WAV.h"



WAV::WAV()
{
	ckID[4] = '\0';
	ckSize[4] = '\0';
	waveid[4] = '\0';

	data = new std::vector<char>;
}


WAV::~WAV()
{
}

void WAV::Load(std::string filePath) {
	std::string absoluteFilePath = (filePath);
	unsigned char x;

	std::ifstream file(absoluteFilePath, std::ios::binary);
	file >> std::noskipws;

	std::string outstream;

	while (file >> x) {
		data->push_back(x);
		//std::cout << std::hex << (int)x;
		//std::cout << " ";
	}

	printf("\nDone loading file.\n");
}

void WAV::Parse() {
	uint16_t pos = 0;

	char tmpChunkID[5]; // Temp chund id to see when a chunk beings

	dataChunk * nChunk = new dataChunk();
	int chunkPos = -1;

	for (int i = 0; i < 4; i++) { ckID[i] = data->at(i); }
	for (int i = 4; i < 8; i++) { ckSize[i%4] = data->at(i); }
	for (int i = 8; i < 12; i++) { waveid[i%4] = data->at(i); }

	pos = 12;

	uint32_t chunkN = 0;/* int((unsigned char)(ckSize[0]) << 24 |
		(unsigned char)(ckSize[1]) << 16 |
		(unsigned char)(ckSize[2]) << 8 |
		(unsigned char)(ckSize[3]));*/
	for (int i = 0; i < 4; i++) {
		chunkN += ((unsigned char)(ckSize[i]) * pow(16, i));
	}

	while (tmpChunkID != "data" || tmpChunkID != "fact") {
		for (int i = 1; i < 4; i++) {
			tmpChunkID[i - 1] = tmpChunkID[i];
		}
		tmpChunkID[3] = data->at(pos);
		pos++;
	}

	
		for (int i = 1; i < 4; i++) {
			tmpChunkID[i - 1] = tmpChunkID[i];
		}
		//tmpChunkID[3] = x;

		if (tmpChunkID == "data") {
			chunks->push_back(nChunk);
			nChunk->SetID(tmpChunkID);
		}
	


	std::cout << "ckID: " << ckID << std::endl;
	std::cout << "ckSize: " << ckSize << std::endl;
	std::cout << "waveID: " << waveid << std::endl;
}