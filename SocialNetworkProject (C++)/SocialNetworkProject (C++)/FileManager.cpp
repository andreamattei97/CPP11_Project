#include "FileManager.hpp"

FileManager::FileManager(const string& filePath):filePath(filePath)
{
}

unique_ptr<Error> FileManager::ExtractLines(list<string> & linesList)
{
	if (!exists(filePath))
	{
		return make_unique<Error>(Error(fileNotFound, filePath));
	}


	ifstream file;
	file.open(filePath);
	
	if (!file.good()) 
	{
		return make_unique<Error>(Error(errorOpening));
	}

	string newValue;
	while (getline(file, newValue))
	{
		linesList.push_back(newValue);
	}

	file.clear();
	file.close();

	if (file.fail())
	{
		return make_unique<Error>(Error(errorClosing));
	}

	return nullptr;
}

