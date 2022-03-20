#include "NewsDatabase.hpp"


NewsDatabase::NewsDatabase(const string & filePath, condition<int> idValidCondition):file(filePath),idValidCondition(idValidCondition)
{
}

unique_ptr<Error> NewsDatabase::LoadDatabase()
{
	list<string> news;
	unique_ptr<Error> error = file.ExtractLines(news);

	list<string>::iterator iter = news.begin();
	for (; error == nullptr && iter != news.end(); iter++) 
	{
		int id;
		list<int> likes, dislikes;
		Date date;
		string message;

		error = extractor.ExtractNews(*iter, id, date, message, likes, dislikes);
		list<unique_ptr<News>>::iterator newsPosition;
		if (error == nullptr)
		{
			error = InsertNews(id, date, message,newsPosition);
			if (error != nullptr) //add the line to the point of error in case of error
			{
				error = make_unique<Error>(Error(error->GetErrorMessage(), error->GetPointOfError() + " \nLinea: " + *iter));
			}
		}
		
		if (error == nullptr)
		{
			for (int dislike : dislikes)
			{
				if (!(*newsPosition)->AddDislike(dislike))
				{
					error =make_unique<Error>(Error(notValidDislike, *iter + "\nID:" + to_string(dislike)));
				}
			}
		}
		if (error == nullptr)
		{
			for (int like : likes)
			{
				if (!(*newsPosition)->AddLike(like))
				{
					error = make_unique<Error>(Error(notValidLike, *iter + "\nID:" + to_string(like)));
				}
			}
		}
	}
	return error;
}

unique_ptr<Error> NewsDatabase::SaveDatabase()
{
	return file.WriteListFilePtr(database.begin(),database.end());
}

unique_ptr<Error> NewsDatabase::InsertNews(int id, const string & message)
{
	list<unique_ptr<News>>::iterator newsPosition;
	return InsertNews(id,  Date::Today(), message, newsPosition);
}

unique_ptr<Error> NewsDatabase::RemoveNews(const News & news)
{
	bool removed = false;
	database.remove_if([&news,&removed](const unique_ptr<News>& pointer) {
		if (pointer.get() == &news)
		{
			removed = true;
			return true;
		}
		return false;
	});
	if (removed)
	{
		return unique_ptr<Error>();
	}
	else
	{
		return make_unique<Error>("Notizia non presente nel database");
	}
}

unique_ptr<Error> NewsDatabase::RemoveAllUserNews(int id)
{
	unique_ptr<Error> error;
	if (!idValidCondition(id))
	{
		error = make_unique<Error>(Error("Id non valid", "Id:" + to_string(id)));

	}
	if (error == nullptr)
	{
		database.remove_if([id](const unique_ptr<News>& news) {
			return id == news->ownerId;
		});
	}
	return error;
}

ShowcaseHandler NewsDatabase::GetHomepageHandler()
{
	return ShowcaseHandler(database.begin(), database.end(), [this](NewsIterator position) {
		return database.erase(position);
	});
}

unique_ptr<UserNewsHandler> NewsDatabase::GetUserNewsHandler(int id)
{
	if(!idValidCondition(id))
		return unique_ptr<UserNewsHandler>();
	unique_ptr<UserNewsHandler> result;
	list<reference_wrapper<News>> userNews;
	for (unique_ptr<News>& news : database)
	{
		if (news->ownerId == id)
			userNews.push_back(ref(*news));
	}
	result = make_unique<UserNewsHandler>(UserNewsHandler(userNews, [this](const News& news) {
		return RemoveNews(news);
	}));
	return result;
}

void NewsDatabase::VisitAllNews(function<void(const News&)> visitFunction) const
{
	for (auto& news : database)
	{
		visitFunction(*news);
	}
}

unique_ptr<Error> NewsDatabase::InsertNews(int id, Date date, const string & message, list<unique_ptr<News>>::iterator& newsPosition)
{
	unique_ptr<Error> error;
	unique_ptr <News> newElement;

	if (!idValidCondition(id))
	{
		error = make_unique<Error>(Error(notValidId, "ID: " + to_string(id)));
	}

	if (error == nullptr && (!date.valid() || date>Date::Today())) //news cannot be in future
	{
		error = make_unique<Error>(Error(notValidDate, date.toString()));
	}

	if (error == nullptr)
	{
		newElement = make_unique<News>(News(id, date, idValidCondition));
	}

	if (error == nullptr && !newElement->SetMessage(message))
	{
		error = make_unique<Error>(Error(notValidMessage, message));
	}

	if (error == nullptr)
	{
		bool exit = false;
		auto iter = database.begin();
		for (; iter != database.end() && !exit; iter++)
		{
			if ((*iter)->date < newElement->date)
			{
				exit = true;
				newsPosition = database.insert(iter, move(newElement));
			}
		}

		if (!exit)
		{
			newsPosition=database.insert(iter,move(newElement));
		}
	}

	return error;
}


