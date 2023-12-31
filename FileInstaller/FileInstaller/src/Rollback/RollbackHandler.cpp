#include "RollbackHandler.h"
#include "../Actions/Action.h"

void RollbackHandler::rollback() {
	for (auto it = _rollbackActions.rbegin(); it != _rollbackActions.rend(); ++it) {
		(*it)->rollback();
	}
}

void RollbackHandler::add_action(std::unique_ptr<Action> rollbackAction) {
	_rollbackActions.push_back(std::move(rollbackAction));
}
