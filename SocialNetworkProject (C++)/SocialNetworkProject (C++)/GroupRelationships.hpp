#ifndef GROUP_USER_RELATIONSHIP_H
#define GROUP_USER_RELATIONSHIP_H

#include "Relationships.hpp"

class GroupRelationships :
	public Relationships
{
public:
	GroupRelationships(int ownerId);

	bool AddRelationship(int id, RelationshipType type) override;
	bool RemoveRelationship(int id, RelationshipType type) override;
	void GetAllRelationships(list<RelationshipData>& relationships) const override;

	const list<int>& GetMembers() const;

private:
	list<int> mMembers;
};

#endif

