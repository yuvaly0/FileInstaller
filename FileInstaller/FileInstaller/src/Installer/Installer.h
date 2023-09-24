#pragma once
#include <string>
#include <vector>
#include "../Path/SourcePath/SourcePath.h"
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Rollback/RollbackHandler.h"

class Installer {
public:
	Installer(DestinationPath* destinationPath, std::vector<std::shared_ptr<SourcePath>> sourcePaths);

	void copy();

private:
	std::vector<std::shared_ptr<SourcePath>> _sourcePaths;
	DestinationPath* _destinationPath;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
};
