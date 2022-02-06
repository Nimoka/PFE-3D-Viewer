#ifndef FILEWITHEXTENSION_H

#include <CLI/CLI.hpp>

// This class checks if the path ends with a given file extension
class FileWithExtension : public CLI::Validator 
{
public:
    FileWithExtension(std::string extension, const std::string &validator_name = std::string{});
};

#define FILEWITHEXTENSION_H
#endif // FILEWITHEXTENSION_H