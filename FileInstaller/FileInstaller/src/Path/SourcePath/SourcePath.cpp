#include "SourcePath.h"
#include "../../Exceptions/InstallerException.h"
#include "../../Utils.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {};

void SourcePath::copy_file(std::shared_ptr<DestinationPath> destinationPath) {
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(destinationPath->_path, _path);

	if (!destinationFilePath) {
		throw InstallerException("couldn't copy file, file path exceeded max size, check to allocate greater path size");
	}
	
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
