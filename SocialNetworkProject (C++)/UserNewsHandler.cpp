#include "UserNewsHandler.hpp"


UserNewsHandler::UserNewsHandler(list<reference_wrapper<News>> userNews, function<unique_ptr<Error>(const News&)> removeFunction):removeFunction(removeFunction)
{
	this->userNews = userNews;
	current = this->userNews.begin();

	auto iter = current;

	for (; elementLimit < 10 && iter != this->userNews.end(); iter++)
	{
		pageElements.push_back(*iter);
		elementLimit++;
	}
}

bool UserNewsHandler::LoadNextPage()
{
	if (elementLimit < 10)
		return false;

	auto iter = current;
	for (int i = 0; i < 10; i++)
	{
		iter++;
	}

	pageElements.clear();
	elementLimit = 0;

	current = iter;

	for (; elementLimit < 10 && iter != userNews.end(); iter++)
	{
		pageElements.push_back(*iter);
		elementLimit++;
	}

	return true;
}

vector<reference_wrapper<const News>> UserNewsHandler::GetPageElements() const
{
	vector<reference_wrapper<const News>> result;

	for (int i = 0; i < elementLimit; i++)
	{
		result.push_back(cref(pageElements[i]));
	}

	return result;
}

bool UserNewsHandler::RemoveNews(int pageElement)
{
	if (pageElement < 0 || pageElement >= elementLimit)
		return false;
	if (removeFunction(pageElements[pageElement]) == nullptr)
	{
		if (pageElement == 0)
		{
			current = userNews.erase(current);
		}
		else
		{
			auto iter = current;
			for (int i = 0; i < pageElement; i++)
				iter++;
			userNews.erase(iter);
		}
		Reload(); 
	}
	else
	{
		return false;
	}

	return true;
}

void UserNewsHandler::Reload()
{
	pageElements.clear();
	int i = 0;
	auto iter = current;
	for (; i < elementLimit && iter != userNews.end(); i++)
	{
		pageElements.push_back(*iter);
		iter++;
	}

	if (iter == userNews.end())
		elementLimit = i;
}
