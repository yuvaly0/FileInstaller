#include <windows.h>
#include <memory>
#include "Installer.h"
#include "../Rollback/Actions/CopiedFile/CopiedFileAction.h"
#include "../Rollback/Actions/CreatedDirRollbackAction/CreatedDirRollbackAction.h"
#include "../Rollback/Actions/CopiedDirRollbackAction/CopiedDirRollbackAction.h"
#include "../Exceptions/InstallerException.h"

Installer::Installer(std::shared_ptr<DestinationPath> destinationPath, std::vector<std::shared_ptr<SourcePath>> sourcePaths)
	: _destinationPath(destinationPath), _sourcePaths(sourcePaths) {
	
	_rollbackHandler = std::make_unique<RollbackHandler>();
	_logger = {};

	auto initializeComResult = CoInitialize(NULL);

	if (FAILED(initializeComResult)) {
		throw InstallerException("couldn't initiailze COM");
	}
}

Installer::~Installer() {
	CoUninitialize();
}

void Installer::rollback() {
	_logger.push_back("starting rollback");

	try {
		_rollbackHandler->rollback();
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
}

void Installer::copy() {
	try {
		DestinationPath::CopyResults result = _destinationPath->tryCreate();

		if (result == DestinationPath::CREATED_DIRECTORY) {
			_logger.push_back("created directory successfully");
			_rollbackHandler->add_action(std::make_unique<CreatedDirRollbackAction>(_destinationPath->_path));
		}

		for (auto i = 0; i < _sourcePaths.size(); i++) {
			std::shared_ptr<SourcePath> currentSourcePath = _sourcePaths.at(i);
			currentSourcePath->copy_path(_destinationPath);
			_logger.push_back("successfully copied path");

			if (currentSourcePath->_isDirectory) {
				_rollbackHandler->add_action(std::make_unique<CopiedDirRollbackAction>(currentSourcePath->_sourcePath, _destinationPath->_path));
			}
			else {
				_rollbackHandler->add_action(std::make_unique<CopiedFileAction>(currentSourcePath->_sourcePath, _destinationPath->_path));
			}
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
	
	rollback();
}