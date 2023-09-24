#pragma once
#include <vector>
#include "./Actions/RollbackAction.h"

class RollbackHandler
{
public:
	void rollback();
	void add_action();

private:
	std::vector<RollbackAction*> rollbackActions;
};

