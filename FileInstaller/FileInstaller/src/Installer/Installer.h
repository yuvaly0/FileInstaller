#pragma once
#include <string>
#include <vector>
#include "../Path/Path.h"

class Installer {
public:
	Installer();

	void copy();

private:
	bool _hasCreatedDestinationFolder;
	std::vector<Path*> _sourcePaths;
	Path* _destinationPath;
};
