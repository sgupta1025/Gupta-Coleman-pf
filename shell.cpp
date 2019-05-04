#include<iostream>
#include <string.h>
#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
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
		dir= get_current_dir_name();
		homeDir = getenv("HOME");
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
		if(num >= 3 && command.find(">") != string::npos)
		{
			int fd = open(strArray[num-1], O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
			if(fd == -1)
			{
				perror("error opening");
			}
			dup2(fd, STDOUT_FILENO);
		}
		if(num >= 3 && command.find(">>") != string::npos)
		{
			int fd = open(strArray[num-1], O_RDWR | O_APPEND);
			if(fd == -1)
			{
				perror("error opening");
			}
			dup2(fd, STDOUT_FILENO);
		}
		if(num >= 3 && command.find("<") != string::npos)
		{
			int fd = open(strArray[num-1], O_RDONLY );
			if(fd == -1)
			{
				perror("error opening");
			}
			dup2(fd, STDIN_FILENO);
		}
		if(strcmp(strArray[0], "exit") == 0)
		{
			break;
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
				execvp(strArray[0], strArray);
				perror("error");
			}
		}
	}
}
