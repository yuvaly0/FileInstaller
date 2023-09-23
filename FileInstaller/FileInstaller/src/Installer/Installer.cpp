#include "Installer.h"

Installer::Installer() {
	_destinationFolder = "C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise";
	_filePathsToCopy = { 
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\1.txt",
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\2.txt",
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\3.txt",
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\4.txt",
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\5.txt",
		"C:\\Users\\yuvalyo\\Documents\\Projects\\winapi exercise\\copyMe\\6.txt"
	};
	_hasCreatedDestinationFolder = false;
}

void Installer::copy() {
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
			
	// 2. 
}