#ifndef NEWS_H
#define NEWS_H

#include <list>
#include <string>

#include "Date.hpp"
#include "Utilities.hpp"

class News
{
public:
	const int ownerId;
	const Date date;

	News(int ownerId, const Date& date, condition<int> validId);

	bool SetMessage(const string& newValue);
	const string& GetMessage() const;


	bool AddLike(int id);
	bool RemoveLike(int id);
	bool AddDislike(int id);
	bool RemoveDislike(int id);

	const list<int>& likesList() const;
	const list<int>& dislikesList() const;

	friend ostream & operator<<(ostream & os, const News& n);

private:
	const condition<int> idCondition;
	list<int> dislikes;
	list<int> likes;

	string message;
	
};

#endif

