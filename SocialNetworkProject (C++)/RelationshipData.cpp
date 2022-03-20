#include "RelationshipData.hpp"

const string RelationshipData::typeStrings[10] = {
	"parent","son","spouse","friend","acquaintance","employer","employee","member","membership","associate"
};

RelationshipData::RelationshipData()
{
}

RelationshipData::RelationshipData(int owner, int target, RelationshipType type): target(target),owner(owner),type(type)
{
}

ostream & operator<<(ostream & os, const RelationshipData & data)
{
	os << data.owner << "," << data.target << "," << RelationshipData::typeStrings[data.type].c_str();
	return os;
}
