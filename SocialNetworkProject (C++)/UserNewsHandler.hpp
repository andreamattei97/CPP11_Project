#ifndef USER_NEWS_HANDLER
#define USER_NEWS_HANDLER

#include <list>
#include <vector>
#include <memory>

#include "News.hpp"
#include "Error.hpp"

//an handler to the news of a single user
class UserNewsHandler
{
public:
	UserNewsHandler(list<reference_wrapper<News>> userNews,function<unique_ptr<Error>(const News&)> removeFunction);

	bool LoadNextPage();

	vector<reference_wrapper<const News>> GetPageElements() const;

	bool RemoveNews(int pageElement);

private:

	const function<unique_ptr<Error>(const News&)> removeFunction;

	list<reference_wrapper<News>> userNews;
	list<reference_wrapper<News>>::iterator current;
	int elementLimit = 0;

	vector<reference_wrapper<News>> pageElements;
	//refresh the current page (called when news are eliminated)
	void Reload();
};

#endif

