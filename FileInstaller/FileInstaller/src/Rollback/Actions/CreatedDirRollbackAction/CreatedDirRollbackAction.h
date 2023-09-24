#pragma once
#include <windows.h>
#include "../RollbackAction.h"

class CreatedDirRollbackAction :
    public RollbackAction
{
public:
    CreatedDirRollbackAction(LPCWSTR folderPath);
    virtual void rollback();

private:
    LPCWSTR _folderPath;
};

