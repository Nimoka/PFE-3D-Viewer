#ifndef TOMLLOADER_H
#define TOMLLOADER_H

#include <toml.hpp>

class TOMLLoader
{
public:
	bool LoadContext(void *c, std::string filepath);
};

#endif // TOMLLOADER_H
