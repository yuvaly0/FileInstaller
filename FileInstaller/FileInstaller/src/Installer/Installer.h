#pragma once
#include <string>
#include <vector>
#include "../Path/SourcePath/SourcePath.h"
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Rollback/RollbackHandler.h"

class Installer {
public:
	Installer(DestinationPath* destinationPath, std::vector<SourcePath*> sourcePaths);

	void copy();

private:
	std::vector<SourcePath*> _sourcePaths;
	DestinationPath* _destinationPath;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
};
