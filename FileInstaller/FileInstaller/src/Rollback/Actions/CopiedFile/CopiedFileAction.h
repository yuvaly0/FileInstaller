#pragma once
#include <windows.h>
#include "../RollbackAction.h"

class CopiedFileAction: public RollbackAction
{
public:
	CopiedFileAction(LPCWSTR sourcePath, LPCWSTR destinationPath);
	virtual void rollback() override;

private:
	LPCWSTR _sourcePath;
	LPCWSTR _destionationPath;
};

