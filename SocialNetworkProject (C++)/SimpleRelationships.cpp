#include "SimpleRelationships.hpp"

SimpleRelationships::SimpleRelationships(int ownerId):Relationships(ownerId,Simple)
{
}

bool SimpleRelationships::AddRelationship(int id, RelationshipType type)
{
	bool result = false;
	if (ownerId != id)
		switch (type)
		{
		case Parent:
			if (mParents.size() < 2 && !Contains(mParents, id))
			{
				mParents.push_front(id);
				result = true;
			}
			break;
		case Son:
			if (!Contains(mSons, id))
			{
				mSons.push_front(id);
				result = true;
			}
			break;
		case Spouse:
			if (!mValidSpouseValue)
			{
				mSpouse = id;
				result = true;
				mValidSpouseValue = true;
			}
			break;
		case Friend:
			if (!Contains(mFriends, id))
			{
				mFriends.push_front(id);
				result = true;
			}
			break;
		case Acquaintance:
			if (!Contains(mAcquaintances, id))
			{
				mAcquaintances.push_front(id);
				result = true;
			}
			break;
		case Employer:
			if (!Contains(mEmployers, id))
			{
				mEmployers.push_front(id);
				result = true;
			}
			break;
		case Membership:
			if (!Contains(mMemberships, id))
			{
				mMemberships.push_front(id);
				result = true;
			}
			break;
		}
	return result;
}

bool SimpleRelationships::RemoveRelationship(int id, RelationshipType type)
{
	bool result = false;
	switch (type)
	{
	case Parent:
		if (Contains(mParents, id))
		{
			mParents.remove(id);
			result = true;
		}
		break;
	case Son:
		if (Contains(mSons, id))
		{
			mSons.remove(id);
			result = true;
		}
		break;
	case Spouse:
		if (mValidSpouseValue && id == mSpouse)
		{
			mValidSpouseValue = false;
			result = true;
		}
		break;
	case Friend:
		if (Contains(mFriends, id))
		{
			mFriends.remove(id);
			result = true;
		}
		break;
	case Acquaintance:
		if (Contains(mAcquaintances, id))
		{
			mAcquaintances.remove(id);
			result = true;
		}
		break;
	case Employer:
		if (Contains(mEmployers, id))
		{
			mEmployers.remove(id);
			result = true;
		}
		break;
	case Membership:
		if (Contains(mMemberships, id))
		{
			mMemberships.remove(id);
			result = true;
		}
	}
	return result;
}

bool SimpleRelationships::AddTargetedAcquaintance(int id)
{
	if (!Contains(mTargetedAcquaintances,id))
	{
		mTargetedAcquaintances.push_front(id);
		return true;
	}
	return false;
}

bool SimpleRelationships::RemoveTargetedAcquaintance(int id)
{
	if (Contains(mTargetedAcquaintances, id))
	{
		mTargetedAcquaintances.remove(id);
		return true;
	}
	return false;
}

bool SimpleRelationships::AddTargetedFriendship(int id)
{
	if (!Contains(targetedFriendships, id))
	{
		targetedFriendships.push_front(id);
		return true;
	}
	return false;
}

bool SimpleRelationships::RemoveTargetedFriendship(int id)
{
	if (Contains(targetedFriendships, id))
	{
		targetedFriendships.remove(id);
		return true;
	}
	return false;
}

const list<int>& SimpleRelationships::GetParents() const
{
	return mParents;
}

const list<int>& SimpleRelationships::GetSons() const
{
	return mSons;
}

bool SimpleRelationships::GetSpouse(int & spouseId) const
{
	spouseId = mSpouse;
	return mValidSpouseValue;

}

const list<int>& SimpleRelationships::GetFriends() const
{
	return mFriends;
}

const list<int>& SimpleRelationships::GetAcquaintances() const
{
	return mAcquaintances;
}

const list<int>& SimpleRelationships::GetEmployers() const
{
	return mEmployers;
}

const list<int>& SimpleRelationships::GetMemberships() const
{
	return mMemberships;
}

const list<int>& SimpleRelationships::GetTargetedAcquaintances() const
{
	return mTargetedAcquaintances;
}

const list<int>& SimpleRelationships::GetTargetedFriendships() const
{
	return targetedFriendships;
}

void SimpleRelationships::GetAllRelationships(list<RelationshipData>& relationships) const
{
	for (int parent : mParents)
	{
		relationships.push_front(RelationshipData(ownerId, parent, Son));
	}
	for (int son : mSons)
	{
		relationships.push_front(RelationshipData(ownerId, son, Parent));
	}
	if (mValidSpouseValue)
	{
		relationships.push_front(RelationshipData(ownerId, mSpouse, Spouse));
	}

	for (int friendId : mFriends)
	{
		relationships.push_front(RelationshipData(ownerId, friendId, Friend));
	}

	for (int acquaintance : mAcquaintances)
	{
		relationships.push_front(RelationshipData(ownerId, acquaintance, Acquaintance));
	}

	for (int employer : mEmployers)
	{
		relationships.push_front(RelationshipData(ownerId, employer, Employee));
	}

	for (int membership : mMemberships)
	{
		relationships.push_front(RelationshipData(ownerId, membership, Member));
	}
}
