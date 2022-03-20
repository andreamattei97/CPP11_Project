#pragma once
#include <string>
#include <list>
#include <fstream>
#include <memory>
#include <algorithm>
#include <filesystem>

#include "Error.hpp"

using namespace std;
using namespace std::experimental::filesystem;

class FileManager
{
public:
	FileManager(const string& filePath);

	unique_ptr<Error> ExtractLines(list<string>& linesList);

	template<typename T>
	unique_ptr<Error> WriteMapFilePtr(T begin, T end); //save a map of pointers

	template<typename T>
	unique_ptr<Error> WriteListFilePtr(T begin, T end); //version for saving list of pointers

	template<typename T>
	unique_ptr<Error> WriteListFile(T begin, T end); //save a list of object (the type of the list must be compatible with the << operator)
	
private:
	
	string errorOpening = "impossibile aprire il file";
	string errorClosing = "impossibile chiude il file correttamente";
	string fileNotFound = "file non trovato";

	const string filePath;
};

template<typename T>
inline unique_ptr<Error> FileManager::WriteMapFilePtr(T begin, T end)
{
	ofstream file;

	file.open(filePath);
	if (!file.good()) 
	{
		return make_unique<Error>(Error(errorOpening));
	}
	

	for (; begin != end; begin++) 
	{
		file << *(begin->second)<<endl;
	}

	file.clear();
	file.close();

	if (file.fail())
	{
		return make_unique<Error>(Error(errorClosing));
	}

	return nullptr;
}

template<typename T>
inline unique_ptr<Error> FileManager::WriteListFilePtr(T begin, T end)
{
	ofstream file;

	file.open(filePath);
	if (!file.good())
	{
		return make_unique<Error>(Error(errorOpening));
	}

	for (; begin != end; begin++)
	{
		file << *(*begin) << endl;
	}

	file.clear();
	file.close();

	if (file.fail())
	{
		return make_unique<Error>(Error(errorClosing));
	}

	return nullptr;
}

template<typename T>
inline unique_ptr<Error> FileManager::WriteListFile(T begin, T end)
{
	ofstream file;

	file.open(filePath);
	if (!file.good())
	{
		return make_unique<Error>(Error(errorOpening));
	}

	for (; begin != end; begin++)
	{
		file << (*begin) << endl;
	}

	file.clear();
	file.close();

	if (file.fail())
	{
		return make_unique<Error>(Error(errorClosing));
	}

	return nullptr;
}
