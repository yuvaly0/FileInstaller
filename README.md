# FileInstaller

FileInstaller is a cpp project written using WINAPI functions and supports the RAII idiom, used for (currently) installing files on a destination path

FileInstaller is atomic, meaning that either it completes his work or he's reverting everything.

## Installation

Use git clone and run the project using visual studio

```bash
git clone https://github.com/yuvaly0/FileInstaller.git
```

## Usage

This installer works in the form of "Actions", each Action is something you would like to do.  
The currently supported actions are
- CreateDirectoryAction
    - Creates a directory and sub-directories as needed, given a path
- CopyPathAction
    - Copies a 'path' given a source and destination paths
    - Able to copy files, empty and non-empty directories
	- Able to handle relative and absolute paths

## Understaing the different components
- Installer
    - This is the core class, it receives one parameter
	```const std::vector<std::unique_ptr<Action>>```, goes over it in a loop and calls *act* for each action
    - After each action, if hasn't threw an exception, we *add* it to a class named "RollbackHandler" 
    - In addition, we preserve in-memory logs for the program (for a case we'd want to send them to a remote server)
 - RollbackHandler
    - Mentioned a bit in the Installer, this class is responsible for handling the 'rollback' logic, it maintains a ```std::vector<std::unique_ptr<Action>>``` for the actions we succeeded running so far, and calls in a loop for *rollback* on each object

## I Want To Add My Own Action
If you'd like to add your own action, you only need to create a class that inherits from the abstract class 'Action' and implement two methods
- act - Does the actual logic of the function (e.g CopyPathAction copies a path)
- rollback - Contains the 'rollback' logic in case other actions have failed after your action has ran

Pay attention that the 'atomicity' concept start from the moment we call the *copy* method of *Installer*, so your action should not *Throw* inside the constructor.  
Tip, you can create an *initialize* private method which you will call in start of your *act*, from which you can *Throw*.

## Where is the rollback occures?
As mentioned, 'FileInstaller' uses the RAII idiom, we refer to each action as a 'Resource'.  
If we got an exception during the running of the actions, we will set a property named *_shouldRollback*, in the dtor of 'Installer' we check this flag, and if needed we call *_rollbackHandler->rollback*.

## Tests
FileInstaller has tests for each Action, make sure to add them if you create your own action

## Example
An example for a main function to look like is 

```cpp
int main() {
	LPCWSTR destinationPath = L".\\copyMe2\\copy3";
	
	// if we initialize "actions" using initialize list, std::vector might try to copy the values which 
	// will result in an error in the case of unique_ptr
	std::vector<std::unique_ptr<Action>> actions;
	actions.push_back(std::make_unique<CreateDirectoryAction>(destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\1.txt", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla", destinationPath));
	actions.push_back(std::make_unique<CopyPathAction>(L"C:\\Users\\yuvalyo\\Documents\\Projects\\\FileInstaller\\copyMe\\bla2", destinationPath));

	auto installer = std::make_unique<Installer>(std::move(actions));
	installer->copy();
}
```

What we see here is a set of actions, that given them the installer will perfrom this actions:
1. Create a directory, if needed, at ".\\copyMe2"
2. Copy the file at "C:\\Users\\yuvalyo\\Documents\\Projects\\FileInstaller\\copyMe\\1.txt" to the created directory
3. Copy the folder at "C:\\Users\\yuvalyo\\Documents\\Projects\\FileInstaller\\copyMe\\bla" to the created directory
4. Copy the folder at "C:\\Users\\yuvalyo\\Documents\\Projects\\FileInstaller\\copyMe\\bla2" to the created directory
5. Copy the folder at "C:\\Users\\yuvalyo\\Documents\\Projects\\FileInstaller\\copyMe\\bla3" to the created directory
