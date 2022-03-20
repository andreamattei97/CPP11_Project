#include "NewsExtractor.hpp"

unique_ptr<Error> NewsExtractor::ExtractNews(const string & newsLine, int & id, Date & date, string & message, list<int>& likes, list<int>& dislikes)
{
	unique_ptr<Error> error;
	 
	string newsDataLine = newsLine;

	if (!regex_match(newsLine, correctNewsLine))
	{
		error = make_unique<Error>(Error(newsLineError, newsLine));
	}

	if (error==nullptr)
	{
		id = ExtractInt(newsDataLine, 0);
		newsDataLine = regex_replace(newsDataLine, numberReplacementRegex, "", regex_constants::format_first_only);

		smatch dislikesResult;
		regex_search(newsDataLine, dislikesResult, dislikesRegex, regex_constants::format_first_only);
		ExtractAllInts(dislikesResult[0], dislikes);
		newsDataLine = regex_replace(newsDataLine, dislikesReplacementRegex, "", regex_constants::format_first_only);

		smatch likesResult;
		regex_search(newsDataLine, likesResult, likesRegex);
		ExtractAllInts(likesResult[0], likes);
		newsDataLine = regex_replace(newsDataLine, likesReplacementRegex, "");

		smatch dateResult;
		regex_search(newsDataLine, dateResult, dateRegex);
		date = ExtractDate(dateResult[0]);
		newsDataLine = regex_replace(newsDataLine, dateReplacementRegex, "");

		message = newsDataLine;
	}

	return error;
}

int NewsExtractor::ExtractInt(const string & line, int position)
{

	sregex_iterator iter(line.begin(), line.end(), numberRegex);
	sregex_iterator end;
	int result;
	int i = 0;
	for (; iter != end && i <= position; iter++)
	{
		if (i == position)
			result = stoi((*iter)[0]);
		i++;
	}

	return result;
}

void NewsExtractor::ExtractAllInts(const string & line, list<int>& list)
{
	sregex_iterator iter(line.begin(), line.end(), numberRegex);
	sregex_iterator end;

	for (; iter != end; iter++)
	{
		list.push_back(stoi((*iter)[0]));
	}
}

Date NewsExtractor::ExtractDate(const string & dateString)
{
	int day = ExtractInt(dateString, 0);
	int month = ExtractInt(dateString, 1);
	int year = ExtractInt(dateString, 2);
	Date result(day, month, year);
	return Date(day, month, year);
}



