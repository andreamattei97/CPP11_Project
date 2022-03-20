#ifndef SIMPLE_USER_RELATIONSHIP_H
#define SIMPLE_USER_RELATIONSHIP_H

#include <list>

#include "Relationships.hpp"

using namespace std;

class SimpleRelationships: public Relationships
{
public:
	SimpleRelationships(int ownerId);

	virtual bool AddRelationship(int id, RelationshipType type) override;
	virtual bool RemoveRelationship(int id, RelationshipType type) override;
	virtual void GetAllRelationships(list<RelationshipData>& relationships) const override;

	bool AddTargetedAcquaintance(int id);
	bool RemoveTargetedAcquaintance(int id);

	bool AddTargetedFriendship(int id);
	bool RemoveTargetedFriendship(int id);

	const list<int>& GetParents()const;
	const list<int>& GetSons() const;
	bool GetSpouse(int& spouseId)const;
	const list<int>& GetFriends() const;
	const list<int>& GetAcquaintances() const;
	const list<int>& GetEmployers() const;
	const list<int>& GetMemberships() const;
	const list<int>& GetTargetedAcquaintances() const;
	const list<int>& GetTargetedFriendships() const;

private:
	list<int> mParents;
	list<int> mSons;
	list<int> mFriends;
	list<int> mAcquaintances;
	list<int> mEmployers;
	list<int> mMemberships;
	list<int> mTargetedAcquaintances;
	list<int> targetedFriendships;

	int mSpouse;
	bool mValidSpouseValue = false;
};

#endif

