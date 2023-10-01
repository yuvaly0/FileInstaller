#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include "../Action.h"

class CreateDirectoryAction : public Action
{
public:
	CreateDirectoryAction(LPCWSTR destinationPath);

	void act() override;
	void rollback() override;

	std::shared_ptr<WCHAR[]> _path;
private:
	void initialize();

	std::vector<std::shared_ptr<WCHAR[]>> _directoriesToBeRemoved;
	bool _hasCreatedDirectory;
};

