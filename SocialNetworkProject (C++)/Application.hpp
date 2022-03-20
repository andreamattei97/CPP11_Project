#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ApplicationInterface.hpp"
#include "RelationshipDatabase.hpp"
#include "UserDatabase.hpp"
#include "NewsDatabase.hpp"
#include "UserManager.hpp"
#include "RelationshipManager.hpp"
#include "NewsManager.hpp"
#include "StatsManager.hpp"
#include "SearchesManager.hpp"

class Application
{
public:
	Application(string usersPath,string relationshipsPath,string newsPath);

	bool LoadDatabases();

	bool run();
	void close();
private:
	ApplicationInterface mInterface;

	UserDatabase userDatabase;
	RelationshipDatabase relationshipDatabase;
	NewsDatabase newsDatabase;

	UserManager userManager;
	RelationshipManager relationshipManager;
	NewsManager newsManager;
	StatsManager statsManager;
	SearchesManager searchesManager;

};

#endif

