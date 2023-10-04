#include <windows.h>
#include <memory>
#include "Installer.h"
#include "../Exceptions/InstallerException.h"

Installer::Installer(const std::vector<std::shared_ptr<Action>>& actions) : _actions(actions) {
	_logger = {};
		
	_rollbackHandler = std::make_unique<RollbackHandler>();

	auto initializeComResult = CoInitialize(NULL);

	if (FAILED(initializeComResult)) {
		throw InstallerException("couldn't initiailze COM");
	}
}

Installer::~Installer() {
	if (_shouldRollback) {
		_rollbackHandler->rollback();
	}

	CoUninitialize();
}

void Installer::copy() {
	try {
		for (auto action : _actions) {
			action->act();
			_logger.push_back("successfully preformed action");
			_rollbackHandler->add_action(action);
		}

		return;
	}
	catch (InstallerException exception) {
		_logger.push_back(exception.what());
	}
	catch (std::exception e) {
		_logger.push_back(e.what());
	}
	catch (...) {
		_logger.push_back("unknown error");
	}
	
	_shouldRollback = true;
}