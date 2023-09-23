#include "DestinationPath.h"

DestinationPath::DestinationPath(LPCWSTR destinationPath) : Path(destinationPath) {};

bool DestinationPath::pre_copy_verify() {
	// 1. checks on the destination
//	* does it exist?
//		* if not
//		  do we have permissions to create it?
//				* if not
//				  abort
//				* if yes
//				  create
// 	        * if yes
//			  is it a directory?
//				* if no 
// 	              abort
// 	            * if yes
// 	              permissions && continue to src paths
//			    
//			  
	//	* do we have write permissions?
	DWORD destinationFileAttributes = GetFileAttributes(_path);

	if (destinationFileAttributes == INVALID_FILE_ATTRIBUTES) {
		return false;
	}

	return true;
}