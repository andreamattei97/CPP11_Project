#include "ShowcaseHandler.hpp"

ShowcaseHandler::ShowcaseHandler(NewsIterator begin, NewsIterator end,function<NewsIterator(NewsIterator)> eraseFunction): begin(begin),end(end),current(begin),eraseFunction(eraseFunction)
{
	elementLimit = 0;

	auto iter = begin;

	for (; elementLimit < 10 && iter != end; iter++)
	{
		pageElements.push_back(ref(**iter));
		elementLimit++;
	}
}


bool ShowcaseHandler::LoadNextPage()
{
	if (elementLimit < 10)
		return false;

	auto iter = current;
	for (int i = 0; i < 10 ; i++) 
	{
		iter++;
	}

	pageElements.clear();
	elementLimit = 0;

	current = iter;

	for (; elementLimit < 10 && iter != end; iter++)
	{
		pageElements.push_back(ref(**iter));
		elementLimit++;
	}

	return true;
}

vector<reference_wrapper<const News>> ShowcaseHandler::GetPageElements() const
{
	vector<reference_wrapper<const News>> result;

	for (int i = 0; i < elementLimit; i++)
	{
		result.push_back(cref(pageElements[i]));
	}

	return result;
}

void ShowcaseHandler::Reload()
{
	pageElements.clear(); 
	int i = 0;
	auto iter = current;
	for (; i < elementLimit && iter != end; i++)
	{
		pageElements.push_back(ref(**iter));
		iter++;
	}

	if (iter == end)
		elementLimit = i;
}

bool ShowcaseHandler::InsertLike(int pageElement, int owner)
{
	if (pageElement < 0 || pageElement >= elementLimit)
	{
		return false;
	}

	return pageElements[pageElement].get().AddLike(owner);
}

bool ShowcaseHandler::InsertDislike(int pageElement, int owner)
{
	if (pageElement < 0 || pageElement >= elementLimit)
	{
		return false;
	}

	return pageElements[pageElement].get().AddDislike(owner);
}

bool ShowcaseHandler::RemoveLike(int pageElement, int owner)
{
	if (pageElement < 0 || pageElement >= elementLimit)
	{
		return false;
	}

	return pageElements[pageElement].get().RemoveLike(owner);
}

bool ShowcaseHandler::RemoveDislike(int pageElement, int owner)
{
	if (pageElement < 0 || pageElement >= elementLimit)
	{
		return false;
	}

	return pageElements[pageElement].get().RemoveDislike(owner);
}

bool ShowcaseHandler::RemoveNews(int pageElement)
{
	if (pageElement < 0 || pageElement >= elementLimit)
		return false;
	if (pageElement == 0)
	{
		current = eraseFunction(current);
		Reload();
	}
	else
	{
		auto iter = current;
		for (int i = 0; i < pageElement; i++)
		{
			iter++;
		}
		eraseFunction(iter);
		Reload();
	}

	return true;
}

