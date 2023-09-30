#pragma once
#include <windows.h>
#include <memory>
#include "../Action.h"

class CreateDirectoryAction : public Action
{
public:
	CreateDirectoryAction(LPCWSTR destinationPath);

	void act() override;
	void rollback() override;

	std::shared_ptr<wchar_t[]> _path;
private:
	void initialize();

	bool _hasCreatedDirectory;
};

