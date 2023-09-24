#include "RollbackHandler.h"

void RollbackHandler::rollback() {
	for (auto it = _rollbackActions.rbegin(); it != _rollbackActions.rend(); ++it) {
		(*it)->rollback();
	}
}

void RollbackHandler::add_action(RollbackAction* action) {
	_rollbackActions.push_back(action);
}
