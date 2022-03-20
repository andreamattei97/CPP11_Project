#include "RelationshipsStatsManager.hpp"

#include <set>

RelationshipsStatsManager::RelationshipsStatsManager(const UserDatabase& userDatabase, const RelationshipDatabase& relationshipDatabase, const ApplicationInterface& applicationInterface):
	userDatabase(userDatabase),relationshipDatabase(relationshipDatabase),applicationInterface(applicationInterface)
{
}


void RelationshipsStatsManager::RelationshipStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Comandi: 0=statistiche utenti semplici, 1=statistiche utenti azienda, 2=statistiche utenti gruppo, 3=esci", true, [](int option) {return option >= 0 && option < 4; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				SimpleRelationshipStatistics();
				break;
			case 1:
				CompanyRelationshipStatistics();
				break;
			case 2:
				GroupRelationshipStatistics();
				break;
			case 3:
				finished = true;
				break;
			}
		}
	}
}

void RelationshipsStatsManager::SimpleRelationshipStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Comandi: 0=parenti e amici (singolo utente), 1=parenti e amici (tutti gli utenti), 2=utente con piu' amici, 3=utente con piu' conoscenti, 4=esci", true, [](int option) {return option >= 0 && option < 5; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				SingleSimpleCount();
				break;
			case 1:
				AllSimpleCount();
				break;
			case 2:
				MostFriends();
				break;
			case 3:
				MostAcquaintances();
				break;
			case 4:
				finished = true;
				break;
			}
		}
	}
}

void RelationshipsStatsManager::CompanyRelationshipStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option,
			"Comandi: 0=dipendenti e consociate singola azienda, 1=dipendenti e consociate tutte le aziende, 2=maggior numero di dipendenti, 3=maggior numero di dipendenti cumulativo, 4=esci",
			true, [](int option) {return option >= 0 && option < 5; });
		if (!aborted)
		{
			switch (option)
			{
			case 0:
				SingleCompanyCount();
				break;
			case 1:
				AllCompanyCount();
				break;
			case 2:
				MostEmployees();
				break;
			case 3:
				MostCumulativeEmployees();
				break;
			case 4:
				finished = true;
				break;
			}
		}
	}
}

void RelationshipsStatsManager::GroupRelationshipStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Comandi: 0=membri singolo gruppo, 1=membri tutti i gruppi, 2=esci", true, [](int option) {return option >= 0 && option < 3; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				SingleGroupCount();
				break;
			case 1:
				AllGroupsCount();
				break;
			case 2:
				finished = true;
				break;
			}
		}
	}
}

void RelationshipsStatsManager::SingleSimpleCount()
{
	int id;
	if (applicationInterface.intInput(id, "Inserisci l'id dell'utente", false,
		[this](int id) {return userDatabase.ContainsId(id) && (userDatabase[id].userType == Simple); }))
	{
		applicationInterface.PrintMessage("Nome utente: " + userDatabase[id].GetUsername());
		int relatives = 0;
		relationshipDatabase.CountRelatives(id,relatives);
		applicationInterface.PrintStatistic("Numero di parenti", relatives, 0);
		int friends = ((const SimpleRelationships&)relationshipDatabase[id]).GetFriends().size();
		applicationInterface.PrintStatistic("Numero di amici", friends, 0);
	}
	else
	{
		applicationInterface.PrintMessage("Id non valido o non appartente ad un utente semplice");
	}
}

void RelationshipsStatsManager::AllSimpleCount()
{
	relationshipDatabase.VisitAllRelationships([this](const Relationships& relationships) {
		if (relationships.ownerType == Simple)
		{
			int relatives = 0;
			relationshipDatabase.CountRelatives(relationships.ownerId, relatives);
			applicationInterface.PrintStatistic("Numero di parenti di "+ userDatabase[relationships.ownerId].GetUsername(), relatives, 0);
			int friends = ((const SimpleRelationships&)relationshipDatabase[relationships.ownerId]).GetFriends().size();
			applicationInterface.PrintStatistic("Numero di amici di " + userDatabase[relationships.ownerId].GetUsername(), friends, 0);
		}
	});
}

void RelationshipsStatsManager::MostFriends()
{
	list<int> results;
	int maxFriends=-1;
	relationshipDatabase.VisitAllRelationships([&results, &maxFriends](const Relationships& relationships) {
		if (relationships.ownerType == Simple)
		{
			if (results.empty())
			{
				results.push_back(relationships.ownerId);
				maxFriends = ((const SimpleRelationships&)relationships).GetFriends().size();
			}
			else
			{
				int friends = ((const SimpleRelationships&)relationships).GetFriends().size();
				if (friends >= maxFriends)
				{
					if (friends > maxFriends)
						results.clear();
					results.push_back(relationships.ownerId);
					maxFriends = friends;
				}
			}
		}
	});
	if (!results.empty())
	{
		applicationInterface.PrintStatistic("Numero massimo di amici trovato", maxFriends, 0);
		applicationInterface.PrintMessage("Utenti:");
		for (int id : results)
		{
			applicationInterface.PrintMessage(userDatabase[id].GetUsername(),1);
		}
	}
}

void RelationshipsStatsManager::MostAcquaintances()
{
	list<int> results;
	int maxAcquaintances = -1;
	relationshipDatabase.VisitAllRelationships([&results, &maxAcquaintances](const Relationships& relationships) {
		if (relationships.ownerType == Simple)
		{
			if (results.empty())
			{
				results.push_back(relationships.ownerId);
				maxAcquaintances = ((const SimpleRelationships&)relationships).GetAcquaintances().size();
			}
			else
			{
				int acquaintances = ((const SimpleRelationships&)relationships).GetAcquaintances().size();
				if (acquaintances >= maxAcquaintances)
				{
					if (acquaintances > maxAcquaintances)
						results.clear();
					results.push_back(relationships.ownerId);
					maxAcquaintances = acquaintances;
				}
			}
		}
	});
	if (!results.empty())
	{
		applicationInterface.PrintStatistic("Numero massimo di conoscenti trovato", maxAcquaintances, 0);
		applicationInterface.PrintMessage("Utenti:");
		for (int id : results)
		{
			applicationInterface.PrintMessage(userDatabase[id].GetUsername(), 1);
		}
	}
}

void RelationshipsStatsManager::SingleCompanyCount()
{
	int id;
	if (applicationInterface.intInput(id, "Inserisci l'id dell'azienda", false,
		[this](int id) {return userDatabase.ContainsId(id) && (userDatabase[id].userType == Company); }))
	{
		applicationInterface.PrintMessage("Nome azienda: " + userDatabase[id].GetName());
		const CompanyRelationships& relationships = (const CompanyRelationships&)relationshipDatabase[id];
		int employees = relationships.GetEmployees().size();
		int associates = relationships.GetAssociates().size();
		applicationInterface.PrintStatistic("Numero di dipendenti dell'azienda", employees, 0);
		applicationInterface.PrintStatistic("Numero di consociate dell'azienda", associates, 0);
	}
	else
	{
		applicationInterface.PrintMessage("Id non valido o non appartente ad un azienda");
	}
}

void RelationshipsStatsManager::AllCompanyCount()
{
	relationshipDatabase.VisitAllRelationships([this](const Relationships& relationships) {
		if (relationships.ownerType == Company)
		{
			const CompanyRelationships& companyRelationships = (const CompanyRelationships&)relationships;
			int employees = companyRelationships.GetEmployees().size();
			int associates = companyRelationships.GetAssociates().size();
			applicationInterface.PrintStatistic("Numero dipendenti di " + userDatabase[relationships.ownerId].GetName(), employees, 0);
			applicationInterface.PrintStatistic("Numero consociate di " + userDatabase[relationships.ownerId].GetName(), associates, 0);
		}
	});
}

void RelationshipsStatsManager::MostEmployees()
{
	list<int> results;
	int storedMaxEmployees = -1;
	relationshipDatabase.VisitAllRelationships([&results, &storedMaxEmployees](const Relationships& relationships) {
		if (relationships.ownerType == Company)
		{
			if (results.empty())
			{
				results.push_back(relationships.ownerId);
				storedMaxEmployees = ((const CompanyRelationships&)relationships).GetEmployees().size();
			}
			else
			{
				int employees = ((const CompanyRelationships&)relationships).GetEmployees().size();
				if (storedMaxEmployees < employees)
				{
					results.clear();
					results.push_back(relationships.ownerId);
					storedMaxEmployees = employees;
				}
				else if (storedMaxEmployees == employees)
				{
					results.push_back(relationships.ownerId);
				}
			}
		}});
	if (results.size() != 0)
	{
		applicationInterface.PrintStatistic("Numero di dipendenti massimo", storedMaxEmployees, 0);
		applicationInterface.PrintMessage("Aziende con piu' dipendenti:");
		for (int id : results)
		{
			applicationInterface.PrintMessage(userDatabase[id].GetName(),1);
		}
	}
}

void RelationshipsStatsManager::MostCumulativeEmployees()
{
	list<int> results,duplicatedResults;
	int maxCounter=-1,maxDuplicatedCounter=-1;
	relationshipDatabase.VisitAllRelationships([&results,&duplicatedResults, &maxCounter,&maxDuplicatedCounter,this](const Relationships& relationships) {
		if (relationships.ownerType == Company)
		{
			if (results.empty())
			{
				results.push_back(relationships.ownerId);
				duplicatedResults.push_back(relationships.ownerId);
				CalculateCumulativeEmployees(relationships.ownerId, maxCounter, maxDuplicatedCounter);
			}
			else
			{
				int counter, duplicatedCounter;
				CalculateCumulativeEmployees(relationships.ownerId, counter, duplicatedCounter);
				if (maxCounter <= counter)
				{
					if (maxCounter < counter)
						results.clear();
					results.push_back(relationships.ownerId);
					maxCounter = counter;
				}

				if (maxDuplicatedCounter <= duplicatedCounter)
				{
					if(maxDuplicatedCounter<duplicatedCounter)
						duplicatedResults.clear();
					duplicatedResults.push_back(relationships.ownerId);
					maxDuplicatedCounter=duplicatedCounter;
				}
			}
		}});
	if (results.size() != 0)
	{
		applicationInterface.PrintStatistic("Numero di dipendenti cumulativi massimo", maxCounter, 0);
		applicationInterface.PrintMessage("Aziende:");
		for (int id : results)
		{
			applicationInterface.PrintMessage(userDatabase[id].GetName(),1);
		}
	}
	if (duplicatedResults.size() != 0)
	{
		applicationInterface.PrintStatistic("Numero di dipendenti cumulativi (considerando i duplicati) massimo", maxDuplicatedCounter, 0);
		applicationInterface.PrintMessage("Aziende:");
		for (int id : duplicatedResults)
		{
			applicationInterface.PrintMessage(userDatabase[id].GetName(),1);
		}
	}
}

void RelationshipsStatsManager::SingleGroupCount()
{
	int id;
	if (applicationInterface.intInput(id, "Inserisci l'id del gruppo", false,
		[this](int id) {return userDatabase.ContainsId(id) && (userDatabase[id].userType == Group); }))
	{
		applicationInterface.PrintMessage("Nome gruppo: " + userDatabase[id].GetName());
		int members = ((const GroupRelationships&)relationshipDatabase[id]).GetMembers().size();
		applicationInterface.PrintStatistic("Numero di membro del gruppo", members, 0);
	}
	else
	{
		applicationInterface.PrintMessage("Id non valido o non appartente ad un gruppo");
	}
}

void RelationshipsStatsManager::AllGroupsCount()
{
	relationshipDatabase.VisitAllRelationships([this](const Relationships& relationships) {
		if (relationships.ownerType == Group)
		{
			int members = ((GroupRelationships&)relationships).GetMembers().size();
			applicationInterface.PrintStatistic("Numero utenti di " + userDatabase[relationships.ownerId].GetName(), members, 0);
		}
	});
}

void RelationshipsStatsManager::CalculateCumulativeEmployees(int id,int & counter, int & duplicatedCounter)
{
	set<int> visitedUsers;
	counter = 0;
	duplicatedCounter = 0;
	const CompanyRelationships& relationships =(const CompanyRelationships&) relationshipDatabase[id];
	for (int id : relationships.GetEmployees())
	{
		counter++;
		duplicatedCounter++;
		visitedUsers.insert(id);
	}
	for (int associate : relationships.GetAssociates())
	{
		for (int id : ((const CompanyRelationships&)relationshipDatabase[associate]).GetEmployees())
		{
			duplicatedCounter++;
			if (visitedUsers.find(id)==visitedUsers.end())
			{
				visitedUsers.insert(id);
				counter++;
			}
		}
	}
}
