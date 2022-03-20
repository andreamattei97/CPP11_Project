#ifndef SHOWCASE_HANDLER_HPP
#define SHOWCASE_HANDLER_HPP

#include <list>
#include <vector>
#include <functional>
#include <memory>

#include "News.hpp"

using namespace std;
using NewsIterator = list<unique_ptr<News>>::iterator;

//an handler for the news homepage (sorted by chronological order)
class ShowcaseHandler
{
public:
	ShowcaseHandler(NewsIterator begin, NewsIterator end,function<NewsIterator(NewsIterator)> eraseFunction);

	bool LoadNextPage();

	bool InsertLike(int pageElement, int owner);
	bool InsertDislike(int pageElement, int owner);

	bool RemoveLike(int pageElement, int owner);
	bool RemoveDislike(int pageElement, int owner);

	bool RemoveNews(int pageElement);

	vector<reference_wrapper<const News>> GetPageElements() const;

private:
	const function<NewsIterator(NewsIterator)> eraseFunction;
	list<unique_ptr<News>>::iterator begin, end, current;
	int elementLimit = 0;

	vector<reference_wrapper<News>> pageElements;

	void Reload();
	
};

#endif

