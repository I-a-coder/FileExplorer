#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm>
using namespace std;


struct Node 
{
    string name;
    bool isDirectory;
    string content;
    Node* firstling;
    Node* sibling;
    Node* dad;

    time_t createdAt;
    time_t modifiedAt;
    size_t fileSize;
    string owner;
    unsigned int permissions;
    bool isSymLink;
    string linkTarget;

    Node(string n, bool isDir, string own = "user", unsigned int perm = 0755, Node* p = nullptr)
        : name(n), isDirectory(isDir), content(""), firstling(nullptr), sibling(nullptr), dad(p),
        createdAt(time(nullptr)), modifiedAt(time(nullptr)), fileSize(0), owner(own), permissions(perm), isSymLink(false), linkTarget("") {}

    ~Node() {
        if (firstling) delete firstling;
        if (sibling) delete sibling;
    }
};

class FileExplorer 
{
    Node* root;
    Node* myDirectory;

public:
    FileExplorer()
    {
        root = new Node("/", true, "root", 0777);
        myDirectory = root;
    }

    ~FileExplorer() {
        delete root;
    }

    void displayCommands() {
        cout << "\n================= File Explorer Commands =================\n\n";
        cout << "Command Syntax                    Description\n";
        cout << "------------------------------------------------\n";
        cout << "mkdir <dirname>                   Create directory\n";
        cout << "cd <path>                        Change directory\n";
        cout << "pwd                              Show current path\n";
        cout << "ls                               List contents\n";
        cout << "touch <filename> [content]        Create file\n";
        cout << "write <filename> <content>        Write to file\n";
        cout << "cat <filename>                   Show file content\n";
        cout << "rm <name>                        Delete file/dir\n";
        cout << "stat <name>                      Show metadata\n";
        cout << "save <filename>                  Save system state\n";
        cout << "load <filename>                  Load system state\n";
        cout << "find <name>                      Search files/dirs\n";
        cout << "find -i <pattern>                Case-insensitive search\n";
        cout << "grep <content>                   Search file contents\n";
        cout << "ln_s <target> <linkName>        Create symbolic link\n";
        cout << "chmod <path> <permissions>       Change permissions\n";
        cout << "chown <path> <newOwner>          Change ownership\n";
        cout << "rename <oldName> <newName>       Rename file/dir\n";
        cout << "exit                             Exit program\n";
        cout << "\nNote: Use quotes for content: sample -> touch file.txt \"Hello World\"\n";
        cout << "================================================\n\n";
    }


    void mkdir(string name) //make dir
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == name)
            {
                cout << "Error: Directory already exists!" << endl;
                return;
            }
            temp = temp->sibling;
        }

        Node* newDir = new Node(name, true, "user", 0755, myDirectory);
        if (!myDirectory->firstling) 
        {
            myDirectory->firstling = newDir;
        }
        else 
        {
            temp = myDirectory->firstling;
            while (temp->sibling)
            {
                temp = temp->sibling;
            }
            temp->sibling = newDir;
        }
        cout << "Directory '" << name << "' created successfully." << endl;
    }

    void cd(string path)  //change dir
    {
        if (path == "..") //move to dad
        {
            if (myDirectory->dad) myDirectory = myDirectory->dad;
            return;
        }

        if (path == "/") //move to root
        {
            myDirectory = root;
            return;
        }

        Node* temp = myDirectory->firstling; // if path is some name, move to child
        while (temp) 
        {
            if (temp->name == path && temp->isDirectory) 
            {
                myDirectory = temp;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: Directory does not exist!" << endl;
    }

    void pwd() //print working dir
    {
        vector<string> path;
        Node* temp = myDirectory;

        while (temp != root) 
        {
            path.push_back(temp->name);
            temp = temp->dad;
        }

        cout << "/";
        for (int i = path.size() - 1; i >= 0; i--)
        {
            cout << path[i];
            if (i > 0) cout << "/";
        }
        cout << endl;
    }

    void ls() //list all child of working dir
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            cout << temp->name;
            if (temp->isDirectory) cout << "/";
            cout << "  ";
            temp = temp->sibling;
        }
        cout << endl;
    }

    void touch(string name, string content = "") //create file
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == name) 
            {
                cout << "Error: File already exists!" << endl;
                return;
            }
            temp = temp->sibling;
        }

        Node* newFile = new Node(name, false, "user", 0644, myDirectory);
        newFile->content = content;
        newFile->fileSize = content.size();

        if (!myDirectory->firstling)
        {
            myDirectory->firstling = newFile;
        }
        else 
        {
            temp = myDirectory->firstling;
            while (temp->sibling) 
            { 
                temp = temp->sibling; 
            }
            temp->sibling = newFile;
        }
        cout << "File '" << name << "' created successfully." << endl;
    }

    void write(string name, string content) //modify file content
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == name && !temp->isDirectory)
            {
                temp->content = content;
                temp->fileSize = content.size();
                temp->modifiedAt = time(nullptr);
                cout << "File '" << name << "' modified successfully." << endl;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: File not found!" << endl;
    }

    void cat(string name) //print file content
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == name && !temp->isDirectory) 
            {
                cout << temp->content << endl;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: File not found!" << endl;
    }

    void stat(string name) //print metadata
    {
        Node* temp = myDirectory->firstling;
        while (temp)
        {
            if (temp->name == name)
           {
                cout << "Name: " << temp->name << endl;
                cout << "Type: " << (temp->isDirectory ? "Directory" : "File") << endl;
                cout << "Owner: " << temp->owner << endl;
                cout << "Permissions: " << oct << temp->permissions << dec << endl; 
                cout << "Size: " << temp->fileSize << " bytes" << endl;
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &temp->createdAt); //to format and display time
                cout << "Created At: " << buffer;
                ctime_s(buffer, sizeof(buffer), &temp->modifiedAt);
                cout << "Modified At: " << buffer;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: File/Directory not found!" << endl;
    }
    void chmod(string path, unsigned int mode) //change permission
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == path) 
            {
                temp->permissions = mode;
                cout << "Permissions of '" << path << "' updated to " << oct << mode << dec << endl;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: '" << path << "' not found!" << endl;
    }
    void chown(string path, string newOwner) //change ownership
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == path) 
            {
                temp->owner = newOwner;
                cout << "Ownership of '" << path << "' changed to " << newOwner << endl;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: '" << path << "' not found!" << endl;
    }
    void find_i(string pattern)  //case insensitive search
    {
        vector<string> results;
        transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
        findHelper(root, pattern, results);
        if (results.empty()) {
            cout << "No files or directories found matching " << pattern << endl;
        }
        else
        {
            for (const string& path : results) 
            {
                cout << path << endl;
            }
        }
    }
    void grep(string content)  //search specific content
    {
        bool found = true;
        Node* temp = myDirectory->firstling;
        while (temp)
        {
            if (temp->isDirectory && temp->content.find(content) == string::npos)
            {
                cout << "Error: Content not found" << endl;
                found = false;
            }
            temp = temp->sibling;
        }
        if (found) 
        {
            cout << "Content" << content << "found" << endl;
        }
    }
    void ln_s(string target, string linkName) //creating symbolic link
    {
        Node* temp = myDirectory->firstling;
        while (temp)
        {
            if (temp->name == target) 
            {
                Node* symlink = new Node(linkName, false, temp->owner, temp->permissions, myDirectory);
                symlink->isSymLink = true;
                symlink->linkTarget = target;
                cout << "Symbolic link '" << linkName << "' created pointing to '" << target << "'." << endl;
                if (!myDirectory->firstling)
                {
                    myDirectory->firstling = symlink;
                }
                else 
                {
                    Node* last = myDirectory->firstling;
                    while (last->sibling)
                    {
                        last = last->sibling;
                    }
                    last->sibling = symlink;
                }
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: Target file/directory '" << target << "' not found!" << endl;
    }
    void rename(string oldName, string newName) //rename a dir/file
    {
        Node* temp = myDirectory->firstling;
        while (temp) 
        {
            if (temp->name == oldName) 
            {
                temp->name = newName;
                cout << "Renamed '" << oldName << "' to '" << newName << "'." << endl;
                return;
            }
            temp = temp->sibling;
        }
        cout << "Error: '" << oldName << "' not found!" << endl;
    }


    void saveToFile(string filename) //putting to file from memory
    {
        ofstream out(filename);
        if (!out) //if not open
        {
            cout << "Error: Unable to open file for saving." << endl;
            return;
        }
        serializeNode(root, out);
        cout << "File system state saved to " << filename << endl;
        out.close();
    }

    void loadFromFile(string filename) //putting from file in memory
    {
        ifstream in(filename);
        if (!in) 
        {
            cout << "Error: Unable to open file for loading." << endl;
            return;
        }
        delete root;
        root = deserializeNode(in);
        myDirectory = root;
        cout << "File system state loaded from " << filename << endl;
        in.close();
    }

    void find(string name) //searching file/dir with name
    {
        vector<string> results;
        findHelper(root, name, results);
        if (results.empty()) 
        {
            cout << "No files or directories found matching " << name << endl;
        }
        else
        {
            for (const string& path : results) 
            {
                cout << path << endl;
            }
        }
    }
    void rm(string name) //remove dir/file
    {
        Node* temp = myDirectory->firstling;
        Node* prev = nullptr;

        while (temp)
        {
            if (temp->name == name) 
            {
                if (temp->isDirectory && temp->firstling != nullptr)
                {
                    cout << "Error: Directory is not empty!" << endl; //dir must empty before removal
                    return;
                }

                if (prev) 
                {
                    prev->sibling = temp->sibling;
                }
                else 
                {
                    myDirectory->firstling = temp->sibling;
                }

                temp->sibling = nullptr;
                delete temp;
                cout << "Successfully removed '" << name << "'." << endl;
                return;
            }
            prev = temp;
            temp = temp->sibling;
        }
        cout << "Error: '" << name << "' not found!" << endl;
    }

private:
    void serializeNode(Node* node, ofstream& out) //write data to file
    {
        if (!node) return; //leaf node
        out << node->name << " " << node->isDirectory << " " << node->owner << " " << node->permissions << " " << node->fileSize << "\n";
        serializeNode(node->firstling, out);
        serializeNode(node->sibling, out);
    }

    Node* deserializeNode(ifstream& in) //read data from file
    {
        string name;
        bool isDir;
        string owner;
        unsigned int perm;
        size_t fileSize;
        if (!(in >> name >> isDir >> owner >> perm >> fileSize)) return nullptr;

        Node* node = new Node(name, isDir, owner, perm);
        node->fileSize = fileSize;
        node->firstling = deserializeNode(in);
        node->sibling = deserializeNode(in);
        return node;
    }

    void findHelper(Node* node, const string& name, vector<string>& results, string path = "/") //Calculating full path of required node
    {
        if (!node) return; //leaf node

        string currentPath = path + node->name + (node->isDirectory ? "/" : "");
        if (node->name == name) results.push_back(currentPath);

        findHelper(node->firstling, name, results, currentPath);
        findHelper(node->sibling, name, results, path); 
    }
};

int main() 
{
    FileExplorer fs;
    cout << "Welcome to the File Explorer!" << endl;
    cout << "Here are the available commands:" << endl;
    fs.displayCommands();

    string command;
    while (true)
    {
        cout << "> ";
        getline(cin, command);
        istringstream iss(command); //input string stream
        string cmd;
        iss >> cmd;

        if (cmd == "mkdir") 
        {
            string name;
            iss >> name;
            fs.mkdir(name);
        }
        else if (cmd == "cd") 
        {
            string path;
            iss >> path;
            fs.cd(path);
        }
        else if (cmd == "pwd") 
        {
            fs.pwd();
        }
        else if (cmd == "ls") 
        {
            fs.ls();
        }
        else if (cmd == "touch")
        {
            string name, content;
            iss >> name;
            getline(iss, content);
            fs.touch(name, content);
        }
        else if (cmd == "write") 
        {
            string name, content;
            iss >> name;
            getline(iss, content);
            fs.write(name, content);
        }
        else if (cmd == "cat") 
        {
            string name;
            iss >> name;
            fs.cat(name);
        }
        else if (cmd == "stat") 
        {
            string name;
            iss >> name;
            fs.stat(name);
        }
        else if (cmd == "save") 
        {
            string filename;
            iss >> filename;
            fs.saveToFile(filename);
        }
        else if (cmd == "load") 
        {
            string filename;
            iss >> filename;
            fs.loadFromFile(filename);
        }
        else if (cmd == "find") 
        {
            string name;
            iss >> name;
            fs.find(name);
        }
        else if (cmd == "rm") 
        {
            string name;
            iss >> name;
            fs.rm(name);
        }
        else if (cmd == "chmod") 
        {
            string path;
            unsigned int permissions;
            iss >> path >> oct >> permissions;
            fs.chmod(path, permissions);
        }
        else if (cmd == "chown")
        {
            string path, newOwner;
            iss >> path >> newOwner;
            fs.chown(path, newOwner);
        }
        else if (cmd == "find_i") 
        {
            string pattern;
            iss >> pattern;
            fs.find_i(pattern);
        }
        else if (cmd == "grep")
        {
            string content;
            iss >> content;          
            fs.grep(content);
        }
        else if (cmd == "ln_s")
        {
            string target, linkName;
            iss >> target >> linkName;
            fs.ln_s(target, linkName);
        }
        else if (cmd == "rename") 
        {
            string oldName, newName;
            iss >> oldName >> newName;
            fs.rename(oldName, newName);
        }

        else if (cmd == "exit")
        {
            cout << "Exiting File Explorer. Goodbye :)" << endl;
            break;
        }
        else
        {
            cout << "Unknown command! Type a valid command." << endl;
        }
    }

    return 0;
}

