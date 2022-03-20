#ifndef RELATIONSHIP_MANAGER_HPP
#define RELATIONSHIP_MANAGER_HPP

#include <functional>

#include "UserDatabase.hpp"
#include "RelationshipDatabase.hpp"
#include "ApplicationInterface.hpp"

using namespace std;

//application component that visualizes and manages the users' relationships
class RelationshipManager
{
public:
	RelationshipManager(UserDatabase& userDatabase,RelationshipDatabase& relationshipDatabase, ApplicationInterface& applicationInterface);

	void ManageRelationships();

	void SaveDatabase();

private:
	string relationshipNames[10]{
		"genitore", "figlio","coniuge","amico","conoscente","datore di lavoro","impiegato","membro","iscrizione", "associata"
	};

	UserDatabase& userDatabase;
	RelationshipDatabase& relationshipDatabase;
	ApplicationInterface& applicationInterface;

	void VisualizeRelationships(int id);
	void AddRelationship(int id);
	void RemoveRelationship(int id);

};

#endif
