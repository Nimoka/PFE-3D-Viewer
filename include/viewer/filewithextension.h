#ifndef FILEWITHEXTENSION_H
#define FILEWITHEXTENSION_H

#include <CLI/CLI.hpp>

// This class checks if the path ends with a given file extension
class FileWithExtension : public CLI::Validator
{
public:
	FileWithExtension(std::string extension,
			const std::string &validator_name = std::string{});
};

#endif // FILEWITHEXTENSION_H
