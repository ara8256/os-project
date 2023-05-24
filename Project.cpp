#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctime>
#include <cstring>
#include <vector>
#include <fstream>
#include <deque>


#define TOKENSIZE 100


using namespace std;



char buffer[80];

string result;

class CommandLog {
public:
    CommandLog(const string& command, const string& time) : command_(command), time_(time) {}

    string get_command() const {
        return command_;
    }

    string get_time() const {
        return time_;
    }
    
    

private:
    string command_;
    string time_;
 
};




void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);
void ExecuteCommand(char **argv);
void ListFiles();
void DisplayFileContents(const char *filename);
void CopyFile(const char *source, const char *destination);
void MoveFile(const char *source, const char *destination);
void RemoveFile(const char *filename);
void CreateDirectory(const char *dirname);
void CreateBackgroundProcess(char **argv);
void SleepForSeconds(int seconds);
void commandlog_show(const vector<CommandLog>& commands);
void ListProcesses();
void KillProcess(const char *pid);
void ShowProcessInfo(const char *pid);
void displayFileHead(const char *filename, int lines);
void displayUserName();
void echoMessage(char **argv);
void AppendStringToFile(const string& filename, const string& content);



string getCurrentTime()
{
    char buffer[80];
    time_t currentTime = time(nullptr);
    tm* timeInfo = localtime(&currentTime);
    
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return string(buffer);
}

vector<CommandLog> commandHistory;
int main()
{


    char *path2;
    char *arr[250];
    char *Tokenized;
    char input[250];
    char *argv[TOKENSIZE];

    


    while (true)
    {
        cout << "cwushell-> ";
        cin.getline(input, 250);
        StrTokenizer(input, argv);
        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "\n") == 0)
        {
            continue;
            return 1;
        }
     	myExecvp(argv);
       
        string command = input;
        string time = getCurrentTime();
        commandHistory.push_back(CommandLog(command, time));
    }
    return 0;
}



void myExecvp(char **argv)
{
    pid_t pid;
    int status;
    int childStatus;
    pid = fork();
    if (pid == 0)
    {	
       ExecuteCommand(argv); // Call custom command function based on the command na
       exit(0);
      
       
    }
    else if (pid < 0)
    {
        cout << "Something went wrong!" << endl;
      
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
          
    }
}

void StrTokenizer(char *input, char **argv)
{
    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    while (stringTokenized != NULL)
    {
        *argv++ = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }

    *argv = NULL;
}





void ExecuteCommand(char **argv)
{

    if (strcmp(argv[0], "ls") == 0)
    {
 	
       	 ListFiles();
       	
       
   
    }
    else if (strcmp(argv[0], "cat") == 0)
    {
        if (argv[1] != NULL)
        {
      
     DisplayFileContents(argv[1]);
           
            
        }
        else
        {
            cout << "Missing file name for cat command!" << endl;
           
        }
    }
    else if (strcmp(argv[0], "cp") == 0)
    {
        if (argv[1] != NULL && argv[2] != NULL)
        {
        
            CopyFile(argv[1], argv[2]);
            }
        else
        {
            cout << "Missing source or destination file for cp command!" << endl;
           
        }
    }
    else if (strcmp(argv[0], "rename") == 0)
    {
        if (argv[1] != NULL && argv[2] != NULL)
        {  
          MoveFile(argv[1], argv[2]);
         
        }
        else
        {
            cout << "Missing source or destination file for mv command!" << endl;
          
        }
    }
    else if (strcmp(argv[0], "rm") == 0)
    {
        if (argv[1] != NULL)
        {   
             RemoveFile(argv[1]);
            
        }
        else
        {
            cout << "Missing file name for rm command!" << endl;
           
        }
    }
    else if (strcmp(argv[0], "mkdir") == 0)
    {
        if (argv[1] != NULL)
        {  
            CreateDirectory(argv[1]);
            
        }
        else
        {
            cout << "Missing directory name for mkdir command!" << endl;
          
        }
    }
    else if (strcmp(argv[0], "ps") == 0)
    {   
       ListProcesses();
        
    }
    else if (strcmp(argv[0], "kill") == 0)
    {
        if (argv[1] != NULL)
        {   
            KillProcess(argv[1]);
           
        }
        else
        {
            cout << "Missing process ID for kill command!" << endl;
            
        }
    }
    else if (strcmp(argv[0], "info") == 0)
    {
        if (argv[1] != NULL)
        {  
            ShowProcessInfo(argv[1]);
           
        }
        else
        {
            cout << "Missing process ID for info command!" << endl;
            
        }
    }
    else if (strcmp(argv[0], "bg") == 0)
    {
        CreateBackgroundProcess(argv);
    }
    else if (strcmp(argv[0], "sleep") == 0)
    {
        if (argv[1] != NULL)
        {	
            int seconds = atoi(argv[1]);
            SleepForSeconds(seconds);
          
        }
        else
        {
            cout << "Missing duration for sleep command!" << endl;
          
        }

    }
    else if (strcmp(argv[0], "log") == 0)
    { 
      commandlog_show(commandHistory);
   
    }
    else if (strcmp(argv[0], "head") == 0)
        {
        if (argv[1] != NULL)
        {	
		 displayFileHead((argv[1]), 10); // Display the first 10 lines by default
		
        }        
        
        }
            
    else if (strcmp(argv[0], "whoami") == 0)
        {
       displayUserName();
		         
            }
    else if (strcmp(argv[0], "echo") == 0)
        {
       echoMessage(argv);      
		        
            }
    else if (strcmp(argv[0], "append") == 0)
        {
            if (argv[1] == nullptr || argv[2] == nullptr)
            {
                cout << "Error: append <file> <content> is missing" << endl;
                
            }
	  else{
            AppendStringToFile(argv[1], argv[2]);
            
        }
        }
        
    else
    {
        cout << "Command not found!" << endl;
       
    }
}

void ListFiles()
{
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL)
    {
        perror("opendir() error");
        
    }
    while ((entry = readdir(dir)) != NULL)
    {
        cout << entry->d_name << endl;
    }
    closedir(dir);
    
}

void DisplayFileContents(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open() error");
       
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    close(fd);
  
}

void CopyFile(const char *source, const char *destination)
{
    int sourceFD = open(source, O_RDONLY);
    if (sourceFD == -1)
    {
        perror("open() error (source)");
      
    }

    int destFD = open(destination, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (destFD == -1)
    {
        perror("open() error (destination)");
        close(sourceFD);
      
    }

    char buffer[4096];
    ssize_t bytesRead;
    while ((bytesRead = read(sourceFD, buffer, sizeof(buffer))) > 0)
    {
        ssize_t bytesWritten = write(destFD, buffer, bytesRead);
        if (bytesWritten == -1)
        {
            perror("write() error");
            close(sourceFD);
            close(destFD);
          
        }
    }

    close(sourceFD);
    close(destFD);

    cout << "File copied successfully!" << endl;
 
}

void MoveFile(const char *source, const char *destination)
{
    if (rename(source, destination) == -1)
    {
        perror("rename() error");
        
    }

    cout << "File moved successfully!" << endl;
   
}

void  RemoveFile(const char *filename)
{
    if (remove(filename) == -1)
    {
        perror("remove() error");
       
    }

    cout << "File removed successfully!" << endl;
   
}

void CreateDirectory(const char *dirname)
{
    if (mkdir(dirname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1)
    {
        perror("mkdir() error");
       
    }

    cout << "Directory created successfully!" << endl;
    
}

void ListProcesses()
{
    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir == NULL)
    {
        perror("opendir() error");
        
    }
    cout << "PID\t" << "Process Name" << endl;
    while ((entry = readdir(dir)) != NULL)
    {
        if (isdigit(entry->d_name[0]))
        {
            cout << entry->d_name << "\t" << entry->d_name << endl;
        }
    }
    closedir(dir);
    
}

void KillProcess(const char *pid)
{
    int processID = atoi(pid);
    if (kill(processID, SIGKILL) == -1)
    {
        perror("kill() error");
       
    }

    cout << "Process terminated successfully!" << endl;
   
}

void ShowProcessInfo(const char *pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("fopen() error");
      
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        cout << line;
    }

    fclose(file);
   
}

void CreateBackgroundProcess(char **argv)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        if (execvp(argv[1], &argv[1]) == -1)
        {
            perror("execvp() error");
            exit(EXIT_FAILURE);
            
        }
    }
    else if (pid > 0)
    {
        cout << "Background process created with PID: " << pid << endl;
       
    }
    else
    {
        perror("fork() error");
       
    }
}

void SleepForSeconds(int seconds)
{
   sleep(seconds);
   
}



void commandlog_show(const vector<CommandLog>& commands)
{
    for (const auto& log : commands)
    {
        cout << log.get_time() << ": " << log.get_command()<<endl;
    }
   
}


void displayFileHead(const char *filename, int lines)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        for (int i = 0; i < lines && getline(file, line); ++i)
        {
            cout << line << endl;
        }
        file.close();
       	
    }
    else
    {
        cout << "Error opening file: " << filename << endl;
        
    }
}





void displayUserName()
{
    char *username = getlogin();
    if (username != nullptr)
    {
        cout << "Current user: " << username << endl;
        
    }
    else
    {
        cout << "Error retrieving current user." << endl;
       
    }
}

void echoMessage(char **argv)
{
    ++argv; // Skip the "echo" command itself
    while (*argv != NULL)
    {
        cout << *argv++ <<endl;
    }
  
    }
    
    
void AppendStringToFile(const string& filename, const string& content)
{
    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        file << content << endl;
        file.close();
        cout << "String appended to file successfully." << endl;
    }
    else
    {
        cout << "Error: Failed to open the file for appending." << endl;
    }
}
