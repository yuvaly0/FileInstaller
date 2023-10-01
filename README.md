# FileInstaller

FileInstaller is a cpp project written using WINAPI functions and supports the RAII idiom, used for (currently) installing files on a destination path

FileInstaller is atomic, meaning that either it completes his work or he is reverting everything, say one of the copies didn't succeed, he will rollback and delete the previous-created files

## Installation

use git clone and run the project using visual studio

```bash
git clone https://github.com/yuvaly0/FileInstaller.git
```

## Usage

This installer works in the form of "Actions", each Action is something you would like to do, the currently supported actions are
- CreateDirectoryAction
    - This action creates a directory and sub-directories as needed given a path
- CopyPathAction
    - This action copies a 'path' given a source and destination paths
    - Able to copy files, empty and non-empty directories

## Understaing the different components
- Installer
    - This is the class that does the 'heavy lifting', it receives one parameter,
a ```const std::vector<std::shared_ptr<Action>> actions```, goes over it in a loop and calls 'act' for each action
    - After each action, if hasn't threw an exception, we 'add' it to a class named "RollbackHandler" 
    - In addition, we preserve in-memory logs for the program (for a case we'd want to send them to a remote server)
 - RollbackHandler
    - Mentioned a bit in the Installer, this class is responsible for handling the 'rollback' logic, it maintains a ```std::vector<std::shared_ptr<Action>>``` for the actions we succeeded running so far, and calls in a loop for 'rollback' on each object

## I Want To Add My Own Action
If you'd like to add your own action, you only need to create a class that inherits from the abstract class 'Action' and implement two methods
- act - the method which does the actual logic of the function (e.g CopyPathAction copies a path)
- rollback - the method which contains the 'rollback' logic in case other actions have failed after your action has ran
