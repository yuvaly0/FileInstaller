#pragma once
#include <vector>
#include <memory>
#include "./Actions/RollbackAction.h"

class RollbackHandler
{
public:
	void rollback();
	void add_action(std::unique_ptr<RollbackAction> rollbackAction);

private:
	std::vector<std::unique_ptr<RollbackAction>> _rollbackActions;
};

