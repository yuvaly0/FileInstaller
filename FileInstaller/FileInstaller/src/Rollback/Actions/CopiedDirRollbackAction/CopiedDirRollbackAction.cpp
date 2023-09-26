#include <ShObjIdl.h>
#include "atlbase.h"
#include "CopiedDirRollbackAction.h"
#include "../../../Utils/Utils.h"
#include "../../../Exceptions/InstallerException.h"

CopiedDirRollbackAction::CopiedDirRollbackAction(LPCWSTR sourcePath, LPCWSTR destinationPath)
	: _sourcePath(sourcePath), _destinationPath(destinationPath), RollbackAction() {};

void CopiedDirRollbackAction::rollback() {
	// no need to check return value, we've done this before and it worked
	std::unique_ptr<wchar_t[]> destinationDirectory = Utils::getDestinationFilePath(_destinationPath, _sourcePath);

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

		CComPtr<IShellItem> deleteDirShellItem = NULL;
		auto deleteDirShellResult = SHCreateItemFromParsingName(destinationDirectory.get(), NULL, IID_PPV_ARGS(&deleteDirShellItem));

		if (FAILED(deleteDirShellResult)) {
			throw InstallerException("couldn't create destination file path shell item");
		}

		auto setFlagsResult = fileOperation->SetOperationFlags(FOFX_REQUIREELEVATION | FOF_SILENT);

		if (FAILED(setFlagsResult)) {
			throw InstallerException("couldn't set flags on IFileOperation");
		}

		auto deleteItemResult = fileOperation->DeleteItem(deleteDirShellItem, NULL);

		if (FAILED(deleteItemResult)) {
			throw InstallerException("failed queuing delete operation");
		}

		auto performOperationsResult = fileOperation->PerformOperations();

		if (FAILED(performOperationsResult)) {
			throw InstallerException("failed performing actual deletion");
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