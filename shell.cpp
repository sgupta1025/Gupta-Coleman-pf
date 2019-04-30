#include<iostream>
#include <string.h>
#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
extern char** environ;
using namespace std;
int main(int argc, char* argv[], char* envp[])
{
	string command;
	string dir= get_current_dir_name();
	string homeDir = getenv("HOME");
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
		if(dup2(saveIn, STDIN_FILENO) ==-1)
		{
			perror("error");
		}
		if(dup2(saveOut, STDOUT_FILENO) ==-1)
		{
			perror("error");
		}
		cout << "1730sh:" << dir << "$ ";
		getline(cin, command);
		//TODO find < >> or >
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
		int redirVal = -1;
		int flag =0;
		for(int i =0; i < num; i++)
		{
			if(strArray[i] != NULL)
			{
				cout << strArray[i] << endl;
				if(strcmp(strArray[i], ">") == 0)
				{
					redirVal = i;
					flag = O_WRONLY | O_TRUNC | O_CREAT;
				}

				if(strcmp(strArray[i], ">>") == 0)
				{
					redirVal = i;
					flag = O_WRONLY | O_APPEND | O_CREAT;
				}

				if(strcmp(strArray[i], "<") == 0)
				{
					redirVal = i;
					flag = O_RDONLY;
				}
				

			}
		}
		if(redirVal != -1 && flag != 0)
		{
			int fd = open(strArray[redirVal + 1], flag);
			if(fd == -1)
			{
				perror("error");
			}
			if((flag  & O_WRONLY) !=0)
			{
				if(dup2(fd, STDOUT_FILENO) ==-1)
				{
					perror("error");
				}
			}
			else if((flag & O_RDONLY) != 0)
			{
				if(dup2(fd, STDIN_FILENO) == -1)
				{
					perror("error");
				}
			}
			close(fd);
		}
		if(strcmp(strArray[0], "export") == 0)
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
				execvp(strArray[0], strArray);
				perror("error");
			}
		}
	}
}
