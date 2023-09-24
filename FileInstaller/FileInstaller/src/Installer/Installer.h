#pragma once
#include <string>
#include <vector>
#include "../Path/SourcePath/SourcePath.h"
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Rollback/RollbackHandler.h"

class Installer {
public:
	Installer();

	void copy();

private:
	std::vector<SourcePath*> _sourcePaths;
	DestinationPath* _destinationPath;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
};
