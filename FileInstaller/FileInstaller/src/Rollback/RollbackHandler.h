#pragma once
#include <vector>
#include <memory>
#include "../Actions/Action.h"

class RollbackHandler
{
public:
	void rollback();
	void add_action(std::shared_ptr<Action> rollbackAction);

private:
	std::vector<std::shared_ptr<Action>> _rollbackActions;
};

