#include <windows.h>
#include "../RollbackAction.h"

class CopiedDirRollbackAction : public RollbackAction
{
public:
	CopiedDirRollbackAction(LPCWSTR sourcePath, LPCWSTR destinationPath);
	virtual void rollback() override;

private:
	LPCWSTR _sourcePath;
	LPCWSTR _destinationPath;
};
