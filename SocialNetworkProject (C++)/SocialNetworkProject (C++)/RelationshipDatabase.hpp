#ifndef RELATIONSHIP_DATABASE_H
#define RELATIONSHIP_DATABASE_H

#include <string>
#include <map>
#include <memory>

#include "Relationships.hpp"
#include "RelationshipData.hpp"
#include "RelationshipExtractor.hpp"
#include "RelationshipsExplorer.hpp"
#include "SimpleRelationships.hpp"
#include "CompanyRelationships.hpp"
#include "GroupRelationships.hpp"
#include "FileManager.hpp"
#include "Utilities.hpp"

using namespace std;

class RelationshipDatabase
{
public:	
	//token class for invalid indexes
	class ErrorRelationship :public Relationships
	{
	public:
		ErrorRelationship();

		virtual bool AddRelationship(int id, RelationshipType type) override;
		virtual bool RemoveRelationship(int id, RelationshipType type) override;
		virtual void GetAllRelationships(list<RelationshipData>& relationships) const override;
	};

	RelationshipDatabase(const string& relationshipFilePath,condition<int> validId);

	unique_ptr<Error> LoadDatabase();
	unique_ptr<Error> SaveDatabase();

	unique_ptr<Error> AddRelationship(RelationshipData data);
	unique_ptr<Error> RemoveRelationship(RelationshipData data);

	unique_ptr<Error> AddUser(int id,UserType type);
	unique_ptr<Error> RemoveUser(int id);
	
	const Relationships& operator[](int id) const;

	void VisitAllRelationships(function<void(const Relationships&)> visitingFunction) const;

	unique_ptr<Error> CountRelatives(int id, int& counter) const;
	unique_ptr<Error> GetFamilyTree(int id, list<UserGraphData>& tree) const;
	void GetAllFamilyTrees(list<list<UserGraphData>>& trees) const;
	
private:
	string notValidType = "tipo di utente non permesso";
	string invalidId = "id non valido";
	string invalidUserType = "tipo utente non compatibile con la relazione non valido";
	string invalidRelationship = "relazione non valida";
	string parentSonInverted = "relazione non valida: una genitore non può essere figlio del figlio";

	const ErrorRelationship errorResult = ErrorRelationship(); //error token returned for invalid indexes

	map<int, unique_ptr<Relationships>> database;
	FileManager file;
	RelationshipExtractor relationshipExtractor;
	RelationshipsExplorer explorer;
	condition<int> mValidId;

	unique_ptr<Error> AddParenthoodRelationship(int parentId, int sonId);
	unique_ptr<Error> AddMarriageRelationship(int spouse1Id, int spouse2Id);
	unique_ptr<Error> AddFriendshipRelationship(int friend1Id, int friend2Id);
	unique_ptr<Error> AddAcquaintanceshipRelationship(int ownerId, int targetId);
	unique_ptr<Error> AddEmploymentRelationship(int ownerId, int targetId);
	unique_ptr<Error> AddMembershipRelationship(int ownerId, int targetId);
	unique_ptr<Error> AddAssociateRelationship(int ownerId, int targetId);

};

#endif

