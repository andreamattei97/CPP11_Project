#ifndef NEWS_MANAGER_H
#define NEWS_MANAGER_H

#include "NewsDatabase.hpp"
#include "UserDatabase.hpp"
#include "ApplicationInterface.hpp"

//Application component that manages and visualize news
class NewsManager
{
public:
	NewsManager(ApplicationInterface& applicationInterface,NewsDatabase& database,const UserDatabase& userDatabase);

	void ManageNews();

	void SaveDatabase();

private:
	regex messageRegex = regex("([[:alnum:]]|[[:punct:]]|[[:space:]])+");

	NewsDatabase& database;
	ApplicationInterface& applicationInterface;
	const UserDatabase& userDatabase;

	//manages the overall showcase of news
	void Showcase(int id);
	//manages the insertion of likes and dislikes on a single news
	void ManageSingleNews(ShowcaseHandler& handler, int elementNumber, int userId);
	void AddNews(int id);
	void RemoveNews(int id);
	//manage the administrator mode where is possible to delete news
	void AdminMode();

};

#endif

