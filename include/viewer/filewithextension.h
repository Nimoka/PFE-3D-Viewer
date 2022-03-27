#ifndef FILEWITHEXTENSION_H
#define FILEWITHEXTENSION_H

#include <CLI/CLI.hpp>

/**
 * \brief Checks if the path ends with a given file extension
 * 
 * This class inheriting `CLI::Validator` checks if a string given as a
 * command line argument ends with a given file extension.
 */
class FileWithExtension : public CLI::Validator
{
public:
	/**
	 * @brief Constructor of the class, to use with `CLI::Option::check()`.
	 * 
	 * This function checks if a given command line argument ends with the file
	 * extension provided with the function, through the usage of 
	 * `CLI::Option::check()`.
	 * 
	 * @param extension Extension to check for.
	 * @param validator_name Name of the validator used whenever a list of
	 * validators is given for condition checking.
	 */
	FileWithExtension(std::string extension,
			const std::string &validator_name = std::string{});
};

#endif // FILEWITHEXTENSION_H
