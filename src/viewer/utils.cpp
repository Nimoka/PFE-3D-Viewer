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
	if (!file)
		return content;

	const int bufferSize = 2048;
	char buffer[bufferSize];
	while (file.getline(buffer, bufferSize))
		content += std::string(buffer) + '\n';

	file.close();

	return content;
}

bool SaveTextFile(const std::string& path, const std::string& content) {
	std::ofstream file(path.c_str(), std::ios::out);
	if (!file)
		return false;

	file << content;
	file.close();

	return true;
}

std::string GetFunctionCallFromDeclaration(const std::string& content) {
	// Find the function declaration (expect return type) in the text if any
	std::size_t begin = content.find(' ');
	std::size_t end = content.find(')', begin);
	if ((begin == std::string::npos) || (end == std::string::npos))
		return "";

	// Isolate function declaration (for quicker `std::string::find()`)
	std::string functionDeclaration = content.substr((begin + 1),
			(end - begin - 1));

	// Search the end of the function name if any
	begin = functionDeclaration.find('(');
	if (begin == std::string::npos)
		return "";

	// Copy the function name
	std::string functionCall = functionDeclaration.substr(0, (begin + 1));

	// Search the arguments’ names
	begin = functionDeclaration.find(' ', begin);
	while (begin != std::string::npos) {
		// Check if it’s the last one
		end = functionDeclaration.find(',', begin);
		if (end == std::string::npos) {
			// If so, copy the end of the declaration, then stop
			functionCall += functionDeclaration.substr((begin + 1),
					(functionDeclaration.size() - begin - 1));
			break;
		} else {
			// If it’s not, copy the name of the argument and the comma
			functionCall += functionDeclaration.substr((begin + 1),
					(end - begin + 1));
		}
		// Search the next one
		begin = functionDeclaration.find(' ', (end + 2));
	}

	functionCall += ")";

	// Return the function call source code created
	return functionCall;
}
