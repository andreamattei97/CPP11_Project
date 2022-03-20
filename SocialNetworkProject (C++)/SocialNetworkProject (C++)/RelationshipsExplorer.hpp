#ifndef RELATIONSHIP_EXPLORER_H
#define RELATIONSHIP_EXPLORER_H

#include <set>
#include <map>
#include <memory>
#include <string>
#include <functional>

#include "Relationships.hpp"
#include "UserGraphData.hpp"
#include "Error.hpp"

class RelationshipsExplorer
{
public:
	RelationshipsExplorer(function<const Relationships&(int)> getRelationships);

	unique_ptr<Error> CheckValidFamilyTree(int id) const;
	unique_ptr<Error> GetTree(int id, list<UserGraphData>& users) const;
	unique_ptr<Error> CountRelatives(int id,int& counter) const;
private:
	string invalidUserType = "tipo utente non valido (necessario utente semplice)";
	string userNotFound = "utente non trovato";
	string cycleFound = "trovata relazione circolare non valida";
	string notValidTree = "albero non valido (meno di 3 generazioni presenti)";

	function<const Relationships&(int)> getRelationships;

	bool SearchCycle(int id,map<int,UserGraphData>& visisedUsers,set<int>& recursionSet) const;
	void CountElement(int id, int& counter, set<int>& visitedUsers) const;

};

#endif

