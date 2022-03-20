#include "GroupRelationships.hpp"

GroupRelationships::GroupRelationships(int ownerId):Relationships(ownerId,Group)
{
}

bool GroupRelationships::AddRelationship(int id, RelationshipType type)
{	
	bool result = false;
	if (type == Member && ownerId != id && !Contains(mMembers, id))
	{
		mMembers.push_front(id);
		result = true;
	}
	return result;
}

bool GroupRelationships::RemoveRelationship(int id, RelationshipType type)
{
	bool result = false;
	if (type == Member && Contains(mMembers, id))
	{
		mMembers.remove(id);
		result = true;
	}
	return result;
}

const list<int>& GroupRelationships::GetMembers() const
{
	return mMembers;
}

void GroupRelationships::GetAllRelationships(list<RelationshipData>& relationships) const
{
	for (int member : mMembers)
	{
		relationships.push_front(RelationshipData(ownerId, member, Membership));
	}
}
