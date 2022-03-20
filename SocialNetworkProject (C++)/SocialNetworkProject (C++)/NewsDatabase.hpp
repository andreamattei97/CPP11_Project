#ifndef NEWS_DATABASE_HPP
#define NEWS_DATABASE_HPP

#include <string>
#include <memory>
#include <list>
#include <map>

#include "Error.hpp"
#include "News.hpp"
#include "FileManager.hpp"
#include "NewsExtractor.hpp"
#include "ShowcaseHandler.hpp"
#include "UserNewsHandler.hpp"

class NewsDatabase
{
public:
	NewsDatabase(const string& filePath, condition<int> idValidCondition);

	unique_ptr<Error> LoadDatabase();
	unique_ptr<Error> SaveDatabase();

	//insert a news (the current date is used)
	unique_ptr<Error> InsertNews(int id, const string& message);
	unique_ptr<Error> RemoveAllUserNews(int id);

	//used for visualizing all news in chronological order (and it can be also used for eliminating news)
	ShowcaseHandler GetHomepageHandler();
	//used for visualizing the news of a single user in chronological order (and it can be also used for eliminating news)
	unique_ptr<UserNewsHandler> GetUserNewsHandler(int id);

	//navigate through all news 
	void VisitAllNews(function<void(const News&)>visitFunction) const;

	const condition<int> idValidCondition;
private:
	string notValidId = "ID utente non valido";
	string notValidDate = "Data notizia non valida";
	string notValidMessage = "Messaggio non valido (caratteri non validi)";
	string notValidDislike = "ID utente dislike non valido";	
	string notValidLike = "ID utente like non valido";

	list<unique_ptr<News>> database;
	FileManager file;
	NewsExtractor extractor;

	//keep the iterator updated (necessary during the loading operation
	unique_ptr<Error> InsertNews(int id, Date date, const string& message,list<unique_ptr<News>>::iterator& newsPosition);
	//remove the given news (the choice is made using the address)
	unique_ptr<Error> RemoveNews(const News& news);
};

#endif
