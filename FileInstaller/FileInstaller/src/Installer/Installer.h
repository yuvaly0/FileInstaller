#pragma once
#include <string>
#include <vector>
#include "../Path/SourcePath/SourcePath.h"
#include "../Path/DestinationPath/DestinationPath.h"
#include "../Rollback/RollbackHandler.h"

class Installer {
public:
	Installer(std::shared_ptr<DestinationPath> destinationPath, std::vector<std::shared_ptr<SourcePath>> sourcePaths);
	~Installer();

	void copy();

private:
	void rollback();

	std::vector<std::shared_ptr<SourcePath>> _sourcePaths;
	std::shared_ptr<DestinationPath> _destinationPath;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
	std::vector<std::string> _logger;
};
