#ifndef NEWS_EXTRACTOR_H
#define NEWS_EXTRACTOR_H

#include <regex>
#include <memory>

#include "Error.hpp"
#include "News.hpp"

//Extracts news data from a line
class NewsExtractor
{
public:
	unique_ptr<Error> ExtractNews(const string& newsLine, int& id, Date& date, string& message, list<int>& likes, list<int>& dislikes);

private:
	regex correctNewsLine = regex("[[:digit:]]+,([[:alnum:]]|[[:punct:]]|[[:space:]])+,[[:digit:]]+/[[:digit:]]+/[[:digit:]]+,like:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\},dislike:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\}");
    regex numberRegex = regex("[[:digit:]]+");
	regex numberReplacementRegex = regex("[[:digit:]]+,");
    regex dislikesRegex = regex("(dislike:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\})$");
	regex dislikesReplacementRegex = regex("(,dislike:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\})$");
	regex likesRegex = regex("(like:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\})$");
	regex likesReplacementRegex = regex("(,like:\\{(([[:digit:]]+,)*[[:digit:]]+)?\\})$");
	regex dateRegex = regex("([[:digit:]]+/[[:digit:]]+/[[:digit:]]+)$");
	regex dateReplacementRegex = regex("(,[[:digit:]]+/[[:digit:]]+/[[:digit:]]+)$");

	string newsLineError = "notizia non valida";

	int ExtractInt(const string& line, int position);
	void ExtractAllInts(const string& line, list<int>& list);
	Date ExtractDate(const string & dateString);
};

#endif
