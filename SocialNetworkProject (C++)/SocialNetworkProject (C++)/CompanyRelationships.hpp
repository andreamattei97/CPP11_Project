#ifndef COMPANY_RELATIONSHIPS_HPP
#define COMPANY_RELATIONSHIPS_HPP

#include <list>

#include "Relationships.hpp"

class CompanyRelationships :
	public Relationships
{
public:
	CompanyRelationships(int ownerId);

    bool AddRelationship(int id, RelationshipType type) override;
	bool RemoveRelationship(int id, RelationshipType type) override;	
	virtual void GetAllRelationships(list<RelationshipData>& relationships) const override;

	const list<int>& GetEmployees() const;
	const list<int>& GetAssociates() const;
private:
	list<int> mEmployees;
	list<int> mAssociates;

};

#endif

