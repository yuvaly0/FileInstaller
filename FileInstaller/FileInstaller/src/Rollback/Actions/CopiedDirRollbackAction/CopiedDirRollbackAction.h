#include <windows.h>
#include <memory>
#include "../RollbackAction.h"

class CopiedDirRollbackAction : public RollbackAction
{
public:
	CopiedDirRollbackAction(LPCWSTR sourcePath, std::shared_ptr<wchar_t[]> destinationPath);
	virtual void rollback() override;

private:
	// todo: i think it's redundant
	LPCWSTR _sourcePath;
	std::shared_ptr<wchar_t[]> _destinationPath;
};
