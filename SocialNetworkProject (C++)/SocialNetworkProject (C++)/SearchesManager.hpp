#ifndef SEARCHES_MANAGER_HPP
#define SEARCHES_MANAGER_HPP

#include "UserDatabase.hpp"
#include "NewsDatabase.hpp"
#include "RelationshipDatabase.hpp"
#include "ApplicationInterface.hpp"


//application component that used for querying the databases
class SearchesManager
{
public:
	SearchesManager(const UserDatabase& userDatabase, const RelationshipDatabase& relationshipDatabase, const NewsDatabase& newsDatabase, const ApplicationInterface& applicationInterface);
	
	void ManageSearches();

private:

	//a struct used for storing temporary data during a sympathy search
	struct NewsNavigationData { 
		list<int> associates;
		int likes=0;
		int dislikes=0;
		int id = 0;
	};

	const UserDatabase& userDatabase;
	const RelationshipDatabase& relationshipDatabase;
	const NewsDatabase& newsDatabase;
	const ApplicationInterface& applicationInterface;

	void FamiltyTree();
	void LoneWolves();
	void SympathySearch();

	void SingleTree();
	void AllTrees();

	//returns the string of a user of the tree
	string userDataString(const UserGraphData& user);
};

#endif 