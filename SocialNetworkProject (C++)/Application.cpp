#include "Application.hpp"

Application::Application(string usersPath, string relationshipsPath, string newsPath):
	userDatabase(usersPath),
	relationshipDatabase(relationshipsPath, bind(&UserDatabase::ContainsId, &userDatabase, placeholders::_1)), //initializes all components
	newsDatabase(newsPath, bind(&UserDatabase::ContainsId, &userDatabase, placeholders::_1)),
	userManager(mInterface,userDatabase),
	relationshipManager(userDatabase,relationshipDatabase,mInterface),
	newsManager(mInterface,newsDatabase,userDatabase),
	statsManager(userDatabase,relationshipDatabase,newsDatabase,mInterface),
	searchesManager(userDatabase,relationshipDatabase,newsDatabase,mInterface)
{
	userDatabase.SetAddCallback([this](const User& newUser) 
	{ 
		return relationshipDatabase.AddUser(newUser.id, newUser.userType); 
	});

	userDatabase.SetRemoveCallback([this](const User& user) {
		unique_ptr<Error> error = newsDatabase.RemoveAllUserNews(user.id);
		error = relationshipDatabase.RemoveUser(user.id);
		return error;
	});

}

bool Application::LoadDatabases()
{
	unique_ptr<Error> error;
	error = userDatabase.LoadDatabase();
	if (error == nullptr)
		error = relationshipDatabase.LoadDatabase();
	if (error == nullptr)
		error = newsDatabase.LoadDatabase();
	if (error != nullptr)
	{
		mInterface.PrintError(*error);
		return false;
	}

	return true;
}

bool Application::run()
{
	bool result = true;
	int command;
	if (mInterface.intInput(command, "Comandi: 0=esci, 1=gestione utenti, 2=gestione relazioni, 3=lettura e gestione notizie, 4=statistiche, 5=ricerche",false, [](int i) {return i >= 0 && i < 6; }))
	{
		switch (command)
		{
		case 0:
			close();
			result = false;
			break;
		case 1:
			userManager.ManageUsers();
			userManager.SaveDatabase();
			newsManager.SaveDatabase();
			relationshipManager.SaveDatabase();
			break;
		case 2:
			relationshipManager.ManageRelationships();
			relationshipManager.SaveDatabase();
			break;
		case 3:
			newsManager.ManageNews();
			newsManager.SaveDatabase();
			break;
		case 4:
			statsManager.ManageStatistics();
			break;
		case 5:
			searchesManager.ManageSearches();
			break;
		}
	}
	return result;
}

void Application::close()
{
	cout << "Programma chiuso con successo";
}


