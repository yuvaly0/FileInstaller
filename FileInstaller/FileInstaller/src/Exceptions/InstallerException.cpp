#include "InstallerException.h"

InstallerException::InstallerException(const std::string& msg) : _msg(msg), std::exception() {}

const char* InstallerException::what() const {
	return _msg.c_str();
}
