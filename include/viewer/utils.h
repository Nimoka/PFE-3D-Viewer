#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#ifdef _WIN32
#define PATH_DELIMITER '\\'
#else
#define PATH_DELIMITER '/'
#endif

namespace Global {
	extern unsigned int nextModuleID;
};

bool FileExists(std::string path);
std::string LoadTextFile(const std::string& path);
bool SaveTextFile(const std::string& path, const std::string& content);

std::string GetFunctionCallFromDeclaration(const std::string& content);

#endif // UTILS_H
