#include<iostream>
#include <string.h>
#include <stdio.h>
#include<unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;
int main(int argc, char* argv[], char* envp[])
{
	string command;
	string dir= get_current_dir_name();
	string homeDir = getenv("HOME");
	char* strArray[11];
	char* token;
	do
	{

		if(dir.find(homeDir) != string::npos)
		{
			dir.replace(dir.find(homeDir), homeDir.length(), "~");
		}
		cout << "1730sh:" << dir << "$ ";
		cin >> command;
		token = strtok(&command[0], " ");
		int num =0;
		while (token != NULL)
		{
			//cout << token << endl;
			strArray[num++] = token;
			token = strtok(NULL, " ");
		}
		for(int i =0; i < 10; i++)
		{
			if(i >= num)
			{
				strArray[i] = NULL;
			}
		}
		if(strcmp(strArray[0], "export") == 0)
		{
			cout << "do export" << endl;
		}
		cout << command << endl;
	}
	while(command.compare("exit") !=0);


}
