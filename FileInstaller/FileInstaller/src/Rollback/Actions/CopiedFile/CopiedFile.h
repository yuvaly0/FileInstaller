#pragma once
#include "../RollbackAction.h"

class CopiedFile: public RollbackAction
{
public:
	virtual void rollback() override;
};

