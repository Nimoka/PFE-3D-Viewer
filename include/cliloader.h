#ifndef CLILOADER_H
#define CLILOADER_H

#include "CLI/CLI.hpp"
#include "filewithextension.h"

class CLILoader
{
public:
	int LoadContext(void *c, int argc, char** argv);
};

#endif // CLILOADER_H
