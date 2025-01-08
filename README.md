# FileExplorer

FileExplorer is a simple command-line file management system implemented in C++. It allows users to create, modify, and manage files and directories in a hierarchical structure, similar to a traditional file system. This project is designed to demonstrate basic file operations, directory navigation, and data persistence through serialization.

## Features

- **Directory Management**: Create and navigate directories.
- **File Operations**: Create, read, write, and delete files.
- **Metadata Access**: View file and directory metadata, including permissions, ownership, and timestamps.
- **Search Functionality**: Find files and directories by name, with case-insensitive options.
- **Symbolic Links**: Create symbolic links to files and directories.
- **Permissions Management**: Change file and directory permissions and ownership.
- **Data Persistence**: Save and load the file system state to and from a file.

## Commands

The following commands are available in the FileExplorer:

| Command Syntax                    | Description                                   |
|-----------------------------------|-----------------------------------------------|
| `mkdir <dirname>`                 | Create a directory.                           |
| `cd <path>`                       | Change the current directory.                 |
| `pwd`                             | Show the current working directory.           |
| `ls`                              | List contents of the current directory.       |
| `touch <filename> [content]`      | Create a file with optional content.         |
| `write <filename> <content>`      | Write content to an existing file.           |
| `cat <filename>`                  | Display the content of a file.               |
| `rm <name>`                       | Delete a file or directory.                   |
| `stat <name>`                     | Show metadata of a file or directory.        |
| `save <filename>`                 | Save the current file system state.          |
| `load <filename>`                 | Load a file system state from a file.       |
| `find <name>`                     | Search for files or directories by name.     |
| `find -i <pattern>`               | Case-insensitive search for files or dirs.   |
| `grep <content>`                  | Search for specific content in files.        |
| `ln_s <target> <linkName>`       | Create a symbolic link to a file or directory.|
| `chmod <path> <permissions>`      | Change permissions of a file or directory.   |
| `chown <path> <newOwner>`        | Change ownership of a file or directory.     |
| `rename <oldName> <newName>`     | Rename a file or directory.                  |
| `exit`                            | Exit the File Explorer.                       |

**Note**: Use quotes for content when creating or writing to files, e.g., `touch file.txt "Hello World"`.

