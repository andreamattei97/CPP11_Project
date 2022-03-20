#ifndef RELATIONSHIPS_STATS_MANAGER_HPP
#define RELATIONSHIPS_STATS_MANAGER_HPP

#include "UserDatabase.hpp"
#include "RelationshipDatabase.hpp"
#include "ApplicationInterface.hpp"

//sub-component of the statistics manager that analyze the statistics related to the users' relationships
class RelationshipsStatsManager
{
public:
	RelationshipsStatsManager(const UserDatabase& userDatabase, const RelationshipDatabase& relationshipDatabase, const ApplicationInterface& applicationInterface);

	void RelationshipStatistics();

private:
	const UserDatabase& userDatabase;
	const RelationshipDatabase& relationshipDatabase;
	const ApplicationInterface& applicationInterface;

	//3 area of Relationships statistics
	void SimpleRelationshipStatistics();
	void CompanyRelationshipStatistics();
	void GroupRelationshipStatistics();

	//simple Relationships statistics
	void SingleSimpleCount();
	void AllSimpleCount();
	void MostFriends();
	void MostAcquaintances();

	//company Relationships statistics
	void SingleCompanyCount();
	void AllCompanyCount();
	void MostEmployees();
	void MostCumulativeEmployees();

	//groups Relationships statistics
	void SingleGroupCount();
	void AllGroupsCount();

	void CalculateCumulativeEmployees(int id,int& counter, int& duplicatedCounter);
};

#endif
