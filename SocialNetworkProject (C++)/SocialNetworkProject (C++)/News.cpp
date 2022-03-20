#include "News.hpp"

#include <algorithm>
#include <regex>


bool News::SetMessage(const string & newValue)
{
	if(!regex_match(newValue,regex("([[:alnum:]]|[[:punct:]]|[[:space:]])+")))
		return false;
	
	message = newValue;
	return true;
}

const string & News::GetMessage() const
{
	return message;
}

News::News(int ownerId,const Date& date,condition<int> validId):idCondition(validId),ownerId(ownerId),date(date)
{
}

bool News::AddLike(int id)
{
	if( !idCondition(id) || find(likes.begin(),likes.end(),id)!=likes.end() || find(dislikes.begin(), dislikes.end(), id) != dislikes.end() || id == ownerId)
		return false;
	likes.push_front(id);
	return true;
}

bool News::RemoveLike(int id)
{
	if (find(likes.begin(), likes.end(), id) == likes.end())
		return false;
	likes.remove(id);
	return false;
}

bool News::AddDislike(int id)
{
	if ( !idCondition(id) || find(dislikes.begin(), dislikes.end(), id) != dislikes.end() || find(likes.begin(), likes.end(), id) != likes.end() || id==ownerId)
		return false;
	dislikes.push_front(id);
	return true;
}

bool News::RemoveDislike(int id)
{
	if (find(dislikes.begin(), dislikes.end(), id) == dislikes.end())
		return false;
	dislikes.remove(id);
	return false;
}

const list<int>& News::likesList() const
{
	return likes;
}

const list<int>& News::dislikesList() const
{
	return dislikes;
}


ostream & operator<<(ostream & os, const News & n)
{
	os << n.ownerId << ',' << n.message<<','<<n.date << "," <<"like:{";

	auto likesList = n.likesList();

	auto iterLikes = likesList.cbegin();
	while(iterLikes != likesList.cend())
	{
		os << *iterLikes;
		iterLikes++;
		if (iterLikes != likesList.cend())
			os << ',';
	}
	
	os << "},dislike:{";

	auto dislikesList = n.dislikesList();

	auto iterDislikes = dislikesList.cbegin();
	while (iterDislikes != dislikesList.cend())
	{
		os << *iterDislikes;
		iterDislikes++;
		if (iterDislikes != dislikesList.cend())
			os << ',';
	}

	os << '}';
	return os;
}
