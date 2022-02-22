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

#endif // UTILS_H
