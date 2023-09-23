#pragma once
#include <string>
#include <vector>
#include "../Path/SourcePath/SourcePath.h"
#include "../Path/DestinationPath/DestinationPath.h"

class Installer {
public:
	Installer();

	void copy();

private:
	void revert();

	bool _hasCreatedDestinationFolder;
	std::vector<SourcePath*> _sourcePaths;
	DestinationPath* _destinationPath;
};
