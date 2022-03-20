#ifndef RELATIONSHIP_DATA_H
#define RELATIONSHIP_DATA_H

#include <iostream>

using namespace std;

enum RelationshipType
{
	Parent, Son, Spouse, Friend, Acquaintance, Employer, Employee, Member, Membership, Associate
};

struct RelationshipData
{
	//array used for converting Relationships types in string
	static const string typeStrings[10];

	int target;
	int owner;
	RelationshipType type;

	RelationshipData();
	RelationshipData(int owner, int target, RelationshipType type);

	friend ostream & operator<<(ostream & os,const RelationshipData& data);

};

#endif

