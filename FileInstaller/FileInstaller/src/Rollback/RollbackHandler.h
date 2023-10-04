#pragma once
#include <vector>
#include <memory>
#include "../Actions/Action.h"

class RollbackHandler
{
public:
	void rollback();
	void add_action(std::unique_ptr<Action> rollbackAction);

private:
	std::vector<std::unique_ptr<Action>> _rollbackActions;
};

