#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <regex>
#include <list>

#include "Error.hpp"
#include "User.hpp"

using namespace std;

//Extract user data from a line
class Extractor
{
public:
	unique_ptr<Error> GetError() const; //returns the current error, stored after a failed extraction

	unique_ptr<User> ExtractUser(const string& userLine);

private:
	regex numberRegex = regex("[[:digit:]]+");
	regex typeRegex = regex("(s|c|g)");
	regex correctUserLine = regex("[[:digit:]]+,(s|c|g),\\{([[:alnum:]]+:([[:alnum:]]|/|[[:space:]])+,)*[[:alnum:]]+:([[:alnum:]]|/|[[:space:]])+\\}");
    regex parameterRegex = regex("[[:alnum:]]+:([[:alnum:]]|/|[[:space:]])+");
	regex parameterNameRegex = regex("[[:alnum:]]+:");
	regex dateRegex = regex("[[:digit:]]+/[[:digit:]]+/[[:digit:]]+");


	string userLineError = "sintassi sbagliata dei dati di un utente";
	string invalidDate = "data non valida";
	string unknownParameter = "parametro non riconosciuto";
	string missingParameter = "parametro mancante";

	bool mOnError = false;
	Error mCurrentError;

	int ExtractInt(const string & line,int position);
	string ExtractWord(const string& line, int position,const regex& wordRegex= regex("[[:alnum:]]+"));
	UserType ExtractUserType(const string& userLine);
	Date ExtractData(const string& dateString);
	void ExtractParameter(const string& userLine, list<string>& parameterList);

	unique_ptr<User> ExtractSimpleUser(const string& userLine,int id, list<string>& parameterList);
	unique_ptr<User> ExtractCompanyUser(const string& userLine,int id, list<string>& parameterList);
	unique_ptr<User> ExtractGroupUser(const string& userLine,int id, list<string>& parameterList);

};

#endif

