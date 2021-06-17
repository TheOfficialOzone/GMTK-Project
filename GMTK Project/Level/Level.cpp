

#include "Level.h"

std::ifstream* loadFile(std::string fileName) {
	std::ifstream* readFile = new std::ifstream(fileName);
	return readFile;
}