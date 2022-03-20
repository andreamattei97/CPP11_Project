#include <iostream>
#include <string>

#include "Application.hpp"

using namespace std;

int main(int argc,char* argv[])
{
	if (argc != 4) 
	{
		cerr << "numero di parametri forniti non validi";
		cin.get();
		return 1;
	}

	Application application=Application(argv[1],argv[2],argv[3]);
	if (!application.LoadDatabases())
	{
		cin.get();
		return 1;
	}

	while (application.run());
	return 0;
}

