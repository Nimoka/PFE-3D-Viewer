#ifndef CLILOADER_H
#define CLILOADER_H

#include <CLI/CLI.hpp>

#include "filewithextension.h"

/**
 * @brief Class handling everything related to command line argument parsing.
 * 
 * This class compounds all actions related to command line argument parsing,
 * including the validity of said arguments, by being a bridge with the `CLI11`
 * library.
 */
class CLILoader
{
public:
	/**
	 * @brief Parses and checks command line arguments.
	 * 
	 * Main function of the class, which parses and checks the various
	 * command line arguments passed to the application.
	 * 
	 * @param c Context of the application, used to store data loaded through
	 * the command line.
	 * @param argc Number of arguments passed to the app if used through a
	 * command prompt.
	 * @param argv Collection of strings used to call the app through a command
	 * prompt.
	 * @return int Returns 0 if all data was loaded successfully, otherwise
	 * returns an error code.
	 */
	int LoadContext(void *c, int argc, char** argv);
};

#endif // CLILOADER_H
