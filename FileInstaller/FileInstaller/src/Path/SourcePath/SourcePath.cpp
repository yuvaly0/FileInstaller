#include "SourcePath.h"
#include "../../Exceptions/InstallerException.h"
#include "../../Utils/Utils.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

void SourcePath::copy_file(std::unique_ptr<wchar_t[]> destinationFilePath) {
	const int result = CopyFileExW(_path, destinationFilePath.get(), NULL, NULL, NULL, COPY_FILE_FAIL_IF_EXISTS);

	if (result != 0) {
		return;
	}

	const DWORD copyFileError = GetLastError();

	switch (copyFileError)
	{
		case ERROR_FILE_NOT_FOUND: {
			throw InstallerException("couldn't copy file, file not found");
		}

		case ERROR_ACCESS_DENIED: {
			throw InstallerException("couldn't copy file, not enough permissions");
		}

		case ERROR_ENCRYPTION_FAILED: {
			throw InstallerException("couldn't copy encrypted file");
		}

		case ERROR_FILE_EXISTS: {
			throw InstallerException("couldn't copy file, exists in destination directory");
		}

		default: {
			throw InstallerException("couldn't copy file, unkown error");
		}
	}
}

void SourcePath::copy_path(std::shared_ptr<DestinationPath> destinationPath) {
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(destinationPath->_path, _path);

	if (!destinationFilePath) {
		throw InstallerException("couldn't copy file, file path exceeded max size, check to allocate greater path size");
	}

	const DWORD fileAttributes = GetFileAttributesW(_path);
	
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
		DWORD a = GetLastError();
		throw InstallerException("couldn't copy file, could not get file attributes");
	}

	if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		const int a = 1;
		// handle directory
	}
	else {
		copy_file(std::move(destinationFilePath));
	}
}
