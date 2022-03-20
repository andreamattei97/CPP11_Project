#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include <list>
#include <algorithm>

#include "User.hpp"
#include "RelationshipData.hpp"

using namespace std;

class Relationships
{
public:
	const int ownerId;
	const UserType ownerType;

	Relationships(int ownerId,UserType ownerType);

	virtual bool AddRelationship(int id, RelationshipType type) = 0;
	virtual bool RemoveRelationship(int id, RelationshipType type) = 0;
	virtual void GetAllRelationships(list<RelationshipData>& relationships) const = 0;

	inline bool Contains(const list<int>& container, int id) const
	{
		return  find(container.cbegin(), container.cend(), id) != container.cend();
	}

};

#endif