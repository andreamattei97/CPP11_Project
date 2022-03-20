#ifndef STATS_MANAGER_HPP
#define STATS_MANAGER_HPP

#include "UserDatabase.hpp"
#include "NewsDatabase.hpp"
#include "RelationshipDatabase.hpp"
#include "ApplicationInterface.hpp"
#include "RelationshipsStatsManager.hpp"


//application component for statiscal analysis
class StatsManager
{
public:
	StatsManager(const UserDatabase& userDatabase,const RelationshipDatabase& relationshipDatabase,const NewsDatabase& newsDatabase, const ApplicationInterface& applicationInterface);

	void ManageStatistics();
private:
	const UserDatabase& userDatabase;
	const RelationshipDatabase& relationshipDatabase;
	const NewsDatabase& newsDatabase;
	const ApplicationInterface& applicationInterface;

	RelationshipsStatsManager relationshipsStatsManager;

	//3 macro area of stastics
	void UserStatistics();
	void NewsStatistics();

	//user statistics
	void UserComposition();
	void BirthdateCount();
	void AverageUserAge();

	//news statistics
	void MostLikedNews();
	void MostDislikedNews();

};

#endif

