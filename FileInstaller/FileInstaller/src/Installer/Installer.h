#pragma once
#include <string>
#include <vector>
#include "../Rollback/RollbackHandler.h"
#include "../Actions/CreateDirectoryAction/CreateDirectoryAction.h"
#include "../Actions/CopyPathAction/CopyPathAction.h"

class Installer {
public:
	Installer(std::vector<std::shared_ptr<Action>> actions);
	~Installer();

	void copy();

private:
	std::vector<std::shared_ptr<Action>> _actions;
	std::unique_ptr<RollbackHandler> _rollbackHandler;
	std::vector<std::string> _logger;
	bool _shouldRollback;
};
