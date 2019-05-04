#include<iostream>
#include <string.h>
#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
using namespace std;
string command;
string dir= get_current_dir_name();
string homeDir = getenv("HOME");
void sigHandle(int sigNo)
{
	if(sigNo == SIGINT)
	{
		cout << "\n";
		if(dir.find(homeDir) != string::npos)
		{
			dir.replace(dir.find(homeDir), homeDir.length(), "~");
		}

		cout << "1730sh:" << dir << "$ "  << endl;

	}
}
int main(int argc, char* argv[], char* envp[])
{
	command = "\n";
	if(dir.find(homeDir) != string::npos)
	{
		dir.replace(dir.find(homeDir), homeDir.length(), "~");
	}
	char* strArray[13];
	char* token;
	int saveOut = dup(STDOUT_FILENO);
	int saveIn = dup(STDIN_FILENO);
	while(command.compare("exit") !=0)
	{
		signal(SIGINT, sigHandle);
		dir= get_current_dir_name();
		if(dir.find(homeDir) != string::npos)
		{
			dir.replace(dir.find(homeDir), homeDir.length(), "~");
		}
		if(dup2(saveIn, STDIN_FILENO) ==-1)
		{
			perror("error");
		}
		if(dup2(saveOut, STDOUT_FILENO) ==-1)
		{
			perror("error");
		}
		string prompt = "1730sh:" + dir + "$ ";
		command = readline(prompt.c_str());
		if(command.length() > 0)
		{
			add_history(command.c_str());
		}
			
		token = strtok(&command[0], " ");
		int num =0;
		while (token != NULL)
		{
			strArray[num++] = token;
			token = strtok(NULL, " ");
		}
		for(int i =0; i < 13; i++)
		{
			if(i >= num)
			{
				strArray[i] = NULL;
			}
		}
		for(int i =0; i < 13; i++)
		{
			if(strArray[i] == NULL)
			{
				break;
			}
			if(strcmp(strArray[i], "~") == 0)
			{
				strArray[i] = const_cast<char *>(homeDir.c_str());	
			}


		}
		if(strcmp(strArray[0], "exit") == 0)
		{
			exit(0);
		}
		if(strcmp(strArray[0], "cd") ==0)
		{
			chdir(strArray[1]);
		}
		else if(strcmp(strArray[0], "export") == 0)
		{
			//	cout << "export" << endl;
			putenv(strArray[1]);
		}
		else if(command.compare("exit") != 0)
		{

			int forkVal = fork();
			int status;
			if(forkVal != 0)
			{
				wait(&status);
			}
			else
			{
				bool redirctNeeded = false;
				if(num >= 3 && command.find(">>") != string::npos)
				{
					redirctNeeded = true;
					int fd = open(strArray[num-1], O_RDWR | O_APPEND | O_CREAT, S_IRWXU);
					if(fd == -1)
					{
						perror("error opening");
					}
					dup2(fd, STDOUT_FILENO);
				}
				else if(num >= 3 && command.find(">") != string::npos)
				{
					redirctNeeded = true;
					int fd = open(strArray[num-1], O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
					if(fd == -1)
					{
						perror("error opening");
					}
					dup2(fd, STDOUT_FILENO);
				}
				else if(num >= 3 && command.find("<") != string::npos)
				{
					redirctNeeded = true;
					int fd = open(strArray[num-1], O_RDONLY );
					if(fd == -1)
					{
						perror("error opening");
					}
					dup2(fd, STDIN_FILENO);
				}
				if(redirctNeeded == true)
				{
					char* newArray[11];
					for(int i =0; i < num - 2; i++)
					{
						newArray[i] = strArray[i];
					}
					for(int i = num -2; i < 11; i++)
					{
						newArray[i] = NULL;
					}
					execvp(newArray[0], newArray);
				}
				else
				{
					execvp(strArray[0], strArray);
					perror("error");
				}
			}
		}
	}
}
