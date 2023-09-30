#pragma once
#include <string>
#include <vector>
#include "../Rollback/RollbackHandler.h"
#include "../Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../Actions/CopyPathAction/CopyPathAction.h"

class Installer {
public:
	Installer(LPCWSTR destinationPath, std::vector<LPCWSTR> sourcePaths);
	~Installer();

	void copy();

private:
	void rollback();

	std::vector<std::shared_ptr<CopyPathAction>> _sourcePaths;
	std::shared_ptr<CreateDirectoryAction> _destinationPath;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
	std::vector<std::string> _logger;
	bool _shouldRollback;
};
