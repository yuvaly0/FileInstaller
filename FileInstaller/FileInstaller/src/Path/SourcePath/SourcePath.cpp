#include <ShObjIdl.h>
#include "atlbase.h"
#include "SourcePath.h"
#include "../../Exceptions/InstallerException.h"
#include "../../Utils/Utils.h"

SourcePath::SourcePath(LPCWSTR sourcePath) : Path(sourcePath) {
	_isDirectory = NULL;
};

void SourcePath::copy_file(LPCWSTR destinationPath) {
	std::unique_ptr<wchar_t[]> destinationFilePath = Utils::getDestinationFilePath(destinationPath, _path);

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

void SourcePath::copy_directory(LPCWSTR destinationFilePath) {
	const DWORD fileAttributes = GetFileAttributesW(destinationFilePath);

	if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
		throw InstallerException("cannot copy directory, already exists in the destination");
	}

	auto initializeComResult = CoInitialize(NULL);

	if (FAILED(initializeComResult)) {
		throw InstallerException("couldn't initiailze COM");
	}

	try {
		CComPtr<IFileOperation> fileOperation = NULL;
		HRESULT createFileOperationResult = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileOperation));

		if (FAILED(createFileOperationResult)) {
			throw InstallerException("couldn't create instance of IFileOperation");
		}

		CComPtr<IShellItem> pFrom = NULL;
		CComPtr<IShellItem> pTo = NULL;
		auto sourceShellCreationResult = SHCreateItemFromParsingName(_path, NULL, IID_PPV_ARGS(&pFrom));
		auto toShellCreationResult = SHCreateItemFromParsingName(destinationFilePath, NULL, IID_PPV_ARGS(&pTo));

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
	catch (InstallerException e) {
		CoUninitialize();
		throw e;
	}
	catch (...) {
		CoUninitialize();
	}

	CoUninitialize();
}

void SourcePath::copy_path(std::shared_ptr<DestinationPath> destinationPath) {
	const DWORD fileAttributes = GetFileAttributesW(_path);
	
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
		DWORD a = GetLastError();
		throw InstallerException("couldn't copy file, could not get file attributes");
	}

	if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		_isDirectory = true;
		copy_directory(destinationPath->_path);
	}
	else {
		_isDirectory = false;
		copy_file(destinationPath->_path);
	}
}
