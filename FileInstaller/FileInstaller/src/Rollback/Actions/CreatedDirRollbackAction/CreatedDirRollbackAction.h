#pragma once
#include <windows.h>
#include <memory>
#include "../RollbackAction.h"

class CreatedDirRollbackAction :
    public RollbackAction
{
public:
    CreatedDirRollbackAction(std::shared_ptr<wchar_t[]> destinationPath);
    virtual void rollback();

private:
    std::shared_ptr<wchar_t[]> _destinationPath;
};

