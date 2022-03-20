#include "CompanyRelationships.hpp"

CompanyRelationships::CompanyRelationships(int ownerId):Relationships(ownerId,Company)
{
}

bool CompanyRelationships::AddRelationship(int id, RelationshipType type)
{
	bool result = false;
	if (ownerId != id) 
	{
		switch (type)
		{
		case Employee:
			if (!Contains(mEmployees, id))
			{
				mEmployees.push_front(id);
				result = true;
			}
			break;
		case Associate:
			if (!Contains(mAssociates, id))
			{
				mAssociates.push_front(id);
				result = true;
			}
			break;
		default:
			break;
		}
	}
	return result;
}

bool CompanyRelationships::RemoveRelationship(int id, RelationshipType type)
{
	bool result = false;
	switch (type)
	{
	case Employee:
		if (Contains(mEmployees, id))
		{
			mEmployees.remove(id);
			result = true;
		}
		break;
	case Associate:
		if (Contains(mAssociates, id))
		{
			mAssociates.remove(id);
			result = true;
		}
		break;
	default:
		break;
	}
	return result;
}

void CompanyRelationships::GetAllRelationships(list<RelationshipData>& relationships) const
{
	for (int employee : mEmployees)
	{
		relationships.push_front(RelationshipData(ownerId,employee,Employer));
	}

	for (int associate : mAssociates) {
		relationships.push_front(RelationshipData(ownerId, associate, Associate));
	}
}

const list<int>& CompanyRelationships::GetEmployees() const
{
	return mEmployees;
}

const list<int>& CompanyRelationships::GetAssociates() const
{
	return mAssociates;
}
