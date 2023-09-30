#pragma once
#include <windows.h>
#include <memory>
#include "../RollbackAction.h"

class CopiedFileAction: public RollbackAction
{
public:
	CopiedFileAction(LPCWSTR sourcePath, std::shared_ptr<wchar_t[]> destinationPath);
	virtual void rollback() override;

private:
	LPCWSTR _sourcePath;
	std::shared_ptr<wchar_t[]> _destinationPath;
};

