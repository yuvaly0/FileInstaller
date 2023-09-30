#pragma once
class Action
{
public:
	virtual void act() = 0;
	virtual void rollback() = 0;
};

