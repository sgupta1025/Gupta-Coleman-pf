#include<iostream>
#include <string>
#include<unistd.h>
#include <cstdlib>
using namespace std;
int main(int argc, char* argv[], char* envp[])
{
	string command;
	string dir= get_current_dir_name();
	string homeDir = getenv("HOME");
	do
	{

		if(dir.find(homeDir) != string::npos)
		{
		dir.replace(dir.find(homeDir), homeDir.length(), "~");
		}
		cout << "1730sh:" << dir << "$ ";
		cin >> command;
		cout << command << endl; 
	}
	while(command.compare("exit") !=0);
	

}
