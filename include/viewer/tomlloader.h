#ifndef TOMLLOADER_H
#define TOMLLOADER_H

#include <toml.hpp>

/**
 * @brief Handles everything related to TOML config file parsing.
 * 
 * This class compounds all actions related to TOML config file parsing,
 * including the validity of the file's contents, by being a bridge with the
 * `TOML11` library.
 */
class TOMLLoader
{
public:
	/**
	 * @brief Parses and checks the TOML config file.
	 * 
	 * Main function of the class, which parses and checks the contents of the
	 * TOML config file used with the application.
	 * 
	 * @param c Context of the application, used to store data loaded through
	 * the config file.
	 * @param filepath Path to the config file to load.
	 * @return true All data to parse was succesfully loaded.
	 * @return false An error was encountered while loading the config file.
	 */
	bool LoadContext(void *c, std::string filepath);
};

#endif // TOMLLOADER_H
