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

std::string LoadTextFile(const std::string& path) {
	std::string content = "";

	std::ifstream file(path.c_str(), std::ios::in);
	if (!file) {
		// TODO: Throw an exception?
		return content;
	}

	const int bufferSize = 2048;
	char buffer[bufferSize];
	while (file.getline(buffer, bufferSize))
		content += std::string(buffer) + '\n';

	file.close();

	return content;
}

bool SaveTextFile(const std::string& path, const std::string& content) {
	std::ofstream file(path.c_str(), std::ios::out);
	if (!file) {
		// TODO: Throw an exception?
		return false;
	}

	file << content;
	file.close();

	return true;
}
