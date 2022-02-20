#include "utils.h"

#include <fstream>

unsigned int Global::nextModuleID = 0;

bool FileExists(std::string path) {
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return false;

	file.close();
	return true;
}
