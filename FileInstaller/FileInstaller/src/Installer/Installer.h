#pragma once
#include <string>
#include <vector>
#include "../Path/Path.h"

class Installer {
public:
	Installer();

	void copy();

private:
	void revert();
	bool copy_file(Path* sourcePath);

	bool _hasCreatedDestinationFolder;
	std::vector<Path*> _sourcePaths;
	Path* _destinationPath;
};
