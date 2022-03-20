#include "StatsManager.hpp"

StatsManager::StatsManager(const UserDatabase& userDatabase, const RelationshipDatabase& relationshipDatabase, const NewsDatabase& newsDatabase,const ApplicationInterface& applicationInterface):
	userDatabase(userDatabase),relationshipDatabase(relationshipDatabase),
	newsDatabase(newsDatabase),applicationInterface(applicationInterface),
	relationshipsStatsManager(userDatabase,relationshipDatabase,applicationInterface)
{
}

void StatsManager::ManageStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Scegli la categoria delle statistiche: 0=utenti, 1=relazioni, 2=notizie, 3=esci", true, [](int option) {return option >= 0 && option < 4; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				UserStatistics();
				break;
			case 1:
				relationshipsStatsManager.RelationshipStatistics();
				break;
			case 2:
				NewsStatistics();
				break;
			case 3:
				finished = true;
				break;
			}
		}
	}
}

void StatsManager::UserStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Comandi: 0=composizione utenti, 1=utenti nati dopo una data, 2=eta' media utenti, 3=esci", true, [](int option) {return option >= 0 && option < 4; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				UserComposition();
				break;
			case 1:
				BirthdateCount();
				break;
			case 2:
				AverageUserAge();
				break;
			case 3:
				finished = true;
				break;
			}
		}
	}
}

void StatsManager::NewsStatistics()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Comandi: 0=notizia con piu' mi piace (notizia piu' apprezzata), 1=notizia con piu' non mi piace (notiza con piu' indignazioni), 2=esci", true, [](int option) {return option >= 0 && option < 3; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				MostLikedNews();
				break;
			case 1:
				MostDislikedNews();
				break;
			case 2:
				finished = true;
				break;
			}
		}
	}
}

void StatsManager::UserComposition()
{
	int userCount = 0, simpleUserCount = 0, companyUserCount = 0, groupUserCount = 0;
	userDatabase.VisitAllUsers([&userCount, &simpleUserCount, &companyUserCount, &groupUserCount](const User& user) {
		userCount++;
		switch (user.userType)
		{
		case Simple:
			simpleUserCount++;
			break;
		case Company:
			companyUserCount++;
			break;
		case Group:
			groupUserCount++;
			break;
		}
	});
	applicationInterface.PrintStatistic("Numero di utenti", userCount,0);
	applicationInterface.PrintStatistic("Numero di utenti semplici", simpleUserCount,0);
	applicationInterface.PrintStatistic("Numero di utenti azienda", companyUserCount,0);
	applicationInterface.PrintStatistic("Numero di utenti gruppo", groupUserCount,0);
}

void StatsManager::BirthdateCount()
{
	Date date;
	if (applicationInterface.dateInput(date, "Inserisci la data limite", false))
	{
		int count=0;
		int simpleCount=0,companyCount=0,groupCount = 0;
		userDatabase.VisitAllUsers([&date, &count,&simpleCount,&companyCount,&groupCount](const User& user)
		{
			if (user.GetBirthdate() > date)
			{
				count++;
				switch (user.userType)
				{
				case Simple:
					simpleCount++;
					break;
				case Company:
					companyCount++;
					break;
				case Group:
					groupCount++;
					break;
				}
			}
		});
		applicationInterface.PrintStatistic("Utenti nati/creati dopo la data limite", count,0);
		applicationInterface.PrintStatistic("Utenti semplici nati dopo la data limite", simpleCount, 0);
		applicationInterface.PrintStatistic("Utenti azienda creati dopo la data limite", companyCount, 0);
		applicationInterface.PrintStatistic("Utenti gruppo creati dopo la data limite", groupCount, 0);
	}
}

void StatsManager::AverageUserAge()
{
	Date today = Date::Today();
	int ageSum = 0;
	int userCount = 0;

	userDatabase.VisitAllUsers([&ageSum, &userCount, &today](const User& user) {
		if (user.userType == Simple)
		{
			Date birthday = user.GetBirthdate();
			int age = today.year() - birthday.year();
			//adjust the age to the birthdate
			if (today.month() < birthday.month()) 
			{
				age--;
			}
			else if (today.month() == birthday.month() && today.day() < birthday.day())
			{
				age--;
			}
			ageSum += age;
			userCount++;
		}
	});
	float averageAge = ((float)ageSum) / ((float)userCount);
	applicationInterface.PrintStatistic("Eta' media degli utenti", averageAge);
}

void StatsManager::MostLikedNews()
{
	list<reference_wrapper<const News>> results;
	newsDatabase.VisitAllNews([&results](const News& news) {
		if (results.empty())
		{
			results.push_back(news);
		}
		else
		{
			int newsLikes = news.likesList().size(), storedNewsLikes = results.front().get().likesList().size();
			if (storedNewsLikes < newsLikes)
			{
				results.clear();
				results.push_back(news);
			}
			else if (storedNewsLikes == newsLikes)
			{
				results.push_back(news);
			}
		}
	});
	if (results.size() != 0)
	{
		applicationInterface.PrintStatistic("Numero di mi piace massimo trovato", results.front().get().likesList().size(), 0);
		applicationInterface.PrintMessage("Notizie con mi piace massimi:");
		for (auto& news : results)
		{
			applicationInterface.PrintNews(news.get(), userDatabase[news.get().ownerId].GetUsername(), 1);
		}
	}
}

void StatsManager::MostDislikedNews()
{
	list<reference_wrapper<const News>> results;
	newsDatabase.VisitAllNews([&results](const News& news) {
		if (results.empty())
		{
			results.push_back(news);
		}
		else
		{
			int newsDislikes = news.dislikesList().size(), storedNewsDislikes = results.front().get().dislikesList().size();
			if (storedNewsDislikes < newsDislikes)
			{
				results.clear();
				results.push_back(news);
			}
			else if (storedNewsDislikes == newsDislikes)
			{
				results.push_back(news);
			}
		}
	});
	if (results.size() != 0)
	{
		applicationInterface.PrintStatistic("Numero di non mi piace massimo trovato", results.front().get().dislikesList().size(), 0);
		applicationInterface.PrintMessage("Notizie con non mi piace massimi:");
		for (auto& news : results)
		{
			applicationInterface.PrintNews(news.get(), userDatabase[news.get().ownerId].GetUsername(), 1);
		}
	}
}
