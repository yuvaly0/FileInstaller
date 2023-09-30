#include <ShObjIdl.h>
#include "atlbase.h"
#include "SourcePath.h"
#include "../../Exceptions/InstallerException.h"
#include "../../Utils/Utils.h"

SourcePath::SourcePath(LPCWSTR sourcePath, std::shared_ptr<wchar_t[]> destinationPath) : Path(sourcePath) {
	_sourcePath = sourcePath;
	_destinationPath = destinationPath;
	_destinationFilePath = Utils::getDestinationFilePath(destinationPath.get(), sourcePath);

	if (!_destinationFilePath) {
		throw InstallerException("couldn't copy path, exceeded max size, check to allocate greater path size");
	}

	const DWORD sourcePathAttributes = GetFileAttributesW(_sourcePath);

	if (sourcePathAttributes == INVALID_FILE_ATTRIBUTES) {
		throw InstallerException("couldn't copy path, could not get attributes");
	}

	if (sourcePathAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		_isDirectory = true;

		const DWORD destinationFilePathAttributes = GetFileAttributesW(_destinationFilePath.get());

		if (destinationFilePathAttributes != INVALID_FILE_ATTRIBUTES) {
			throw InstallerException("cannot copy directory, already exists in the destination");
		}
	}
	else {
		_isDirectory = false;
	}
};

void SourcePath::copy_file() {
	const int result = CopyFileExW(_sourcePath, _destinationFilePath.get(), NULL, NULL, NULL, COPY_FILE_FAIL_IF_EXISTS);

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

void SourcePath::copy_directory() {
	CComPtr<IFileOperation> fileOperation = NULL;
	HRESULT createFileOperationResult = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileOperation));

	if (FAILED(createFileOperationResult)) {
		throw InstallerException("couldn't create instance of IFileOperation");
	}

	// todo: should add another null ?
	CComPtr<IShellItem> pFrom = NULL;
	CComPtr<IShellItem> pTo = NULL;
	auto sourceShellCreationResult = SHCreateItemFromParsingName(_sourcePath, NULL, IID_PPV_ARGS(&pFrom));
	auto toShellCreationResult = SHCreateItemFromParsingName(_destinationPath.get(), NULL, IID_PPV_ARGS(&pTo));

	if (FAILED(sourceShellCreationResult) || FAILED(toShellCreationResult)) {
		throw InstallerException("couldn't create shell items paths");
	}

	auto setFlagsResult = fileOperation->SetOperationFlags(FOFX_REQUIREELEVATION | FOF_SILENT | FOFX_EARLYFAILURE | FOF_NOERRORUI);

	if (FAILED(setFlagsResult)) {
		throw InstallerException("couldn't set flags on IFileOperation");
	}

	auto copyItemResult = fileOperation->CopyItem(pFrom, pTo, NULL, NULL);

	if (FAILED(copyItemResult)) {
		throw InstallerException("failed queuing the copy operation");
	}

	auto performOperationsResult = fileOperation->PerformOperations();

	if (FAILED(performOperationsResult)) {
		throw InstallerException("failed performing actual copying");
	}
}

void SourcePath::copy_path(std::shared_ptr<DestinationPath> destinationPath) {
	if (_isDirectory) {
		copy_directory();
	}
	else {
		copy_file();
	}
}
