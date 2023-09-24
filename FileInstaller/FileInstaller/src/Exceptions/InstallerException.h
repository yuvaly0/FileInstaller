#pragma once
#include <exception>
#include <string>

class InstallerException : std::exception
{
public: 
	InstallerException(const std::string& msg);

	virtual const char* what() const override;

private:
	std::string _msg;
};

