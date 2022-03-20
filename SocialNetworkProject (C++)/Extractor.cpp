#include "Extractor.hpp"

#include "SimpleUser.hpp"
#include "CompanyUser.hpp"
#include "GroupUser.hpp"

unique_ptr<Error> Extractor::GetError() const
{
	unique_ptr<Error> result = nullptr;
	if (mOnError)
		result = make_unique<Error>(mCurrentError);
	return result;
}

unique_ptr<User> Extractor::ExtractUser(const string & userLine)
{
	unique_ptr<User> result;

	if (!regex_match(userLine, correctUserLine)) 
	{
		mCurrentError = Error(userLineError, userLine);
		mOnError = true;
	}

	if (!mOnError) {

		int id = ExtractInt(userLine, 0);

		UserType type = ExtractUserType(userLine);

		list<string> paramaters;
		ExtractParameter(userLine, paramaters);

		switch (type)
		{
		case Simple:
			result = ExtractSimpleUser(userLine,id, paramaters);
			break;
		case Company:
			result = ExtractCompanyUser(userLine,id,paramaters);
			break;
		case Group:
			result = ExtractGroupUser(userLine,id,paramaters);
			break;
		}

		if (mOnError == true)
			result.reset();
	}

	return result;
}

int Extractor::ExtractInt(const string & line, int position)
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

string Extractor::ExtractWord(const string & line, int position,const regex& wordRegex)
{
	sregex_iterator iter(line.begin(), line.end(), wordRegex);
	sregex_iterator end;
	string result = "";
	int i = 0;
	for (; iter != end && i<=position; iter++)
	{
		if (i == position)
			result = (*iter)[0];
		i++;
	}

	return result;
}

UserType Extractor::ExtractUserType(const string & userLine)
{
	UserType result;
	smatch regexResult;

	regex_search(userLine, regexResult, typeRegex);

	char t = ((string)regexResult[0])[0];
	switch (t)
	{
	case 's':
		result = Simple;
		break;
	case 'c':
		result = Company;
		break;
	case 'g':
		result = Group;
	}

	return result;
}

Date Extractor::ExtractData(const string & dateString)
{
	int day = ExtractInt(dateString, 0);
	int month = ExtractInt(dateString, 1);
	int year = ExtractInt(dateString, 2);
	Date result(day, month, year);
	return Date(day,month,year);
}

void Extractor::ExtractParameter(const string& userLine, list<string>& parameterList)
{
	sregex_iterator iter(userLine.begin(), userLine.end(), parameterRegex);
	sregex_iterator end;

	for (; iter != end; iter++) 
	{
		parameterList.push_back((*iter)[0]);
	}
}

unique_ptr<User> Extractor::ExtractSimpleUser(const string & userLine,int id, list<string>& parameterList)
{
	unique_ptr<SimpleUser> result = make_unique<SimpleUser>(SimpleUser(id));
	list<string>::iterator iter = parameterList.begin();
	for (; iter != parameterList.end() && !mOnError; iter++) 
	{
		string parameterName = ExtractWord(*iter, 0);
		string paramterValue = regex_replace(*iter,parameterNameRegex,"",regex_constants::format_first_only);
		if (parameterName == "name") 
		{
			result->SetName(paramterValue);
		}
		else if (parameterName == "surname") 
		{
			result->SetSurname(paramterValue);
		}
		else if (parameterName == "birthdate") 
		{
			if (!regex_match(paramterValue, dateRegex) || !(result->SetBirthdate(ExtractData(paramterValue))))
			{
				mCurrentError = Error(invalidDate);
				mOnError = true;
			}
		}
		else if (parameterName == "address") 
		{
			result->SetAddress(paramterValue);
		}
		else 
		{
			mCurrentError = Error(unknownParameter, *iter);
			mOnError = true;
		}
	}

	return move(result);
}

unique_ptr<User> Extractor::ExtractCompanyUser(const string & userLine,int id, list<string>& parameterList)
{
	unique_ptr<CompanyUser> result = make_unique<CompanyUser>(CompanyUser(id));
	list<string>::iterator iter = parameterList.begin();
	for (; iter != parameterList.end() && !mOnError; iter++)
	{
		string parameterName = ExtractWord(*iter, 0);
		string paramterValue = regex_replace(*iter, parameterNameRegex, "", regex_constants::format_first_only);
		if (parameterName == "name")
		{
			result->SetName(paramterValue);
		}
		else if (parameterName == "registeredOffice")
		{
			result->SetRegisteredOffice(paramterValue);
		}
		else if (parameterName == "birthdate")
		{
			if (!regex_match(paramterValue, dateRegex) || !(result->SetBirthdate(ExtractData(paramterValue))))
			{
				mCurrentError = Error(invalidDate);
				mOnError = true;
			}
		}
		else if (parameterName == "headquarter")
		{
			result->SetHeadquarter(paramterValue);
		}
		else if (parameterName == "fieldOfInterest")
		{
			result->SetFieldOfInterest(paramterValue);
		}
		else
		{
			mCurrentError = Error(unknownParameter, *iter);
			mOnError = true;
		}
	}	

	return move(result);
}

unique_ptr<User> Extractor::ExtractGroupUser(const string & userLine,int id, list<string>& parameterList)
{
	unique_ptr<GroupUser> result = make_unique<GroupUser>(GroupUser(id));
	list<string>::iterator iter = parameterList.begin();
	for (; iter != parameterList.end() && !mOnError; iter++)
	{
		string parameterName = ExtractWord(*iter, 0);
		string paramterValue = regex_replace(*iter, parameterNameRegex, "", regex_constants::format_first_only);
		if (parameterName == "name")
		{
			result->SetName(paramterValue);
		}
		else if (parameterName == "address")
		{
			result->SetAddress(paramterValue);
		}
		else if (parameterName == "birthdate")
		{
			if (!regex_match(paramterValue, dateRegex) || !(result->SetBirthdate(ExtractData(paramterValue))))
			{
				mCurrentError = Error(invalidDate);
				mOnError = true;
			}
		}
		else if (parameterName == "areaOfOperation")
		{
			result->SetAreaOfOperation(paramterValue);
		}
		else
		{
			mCurrentError = Error(unknownParameter, *iter);
			mOnError = true;
		}
	}	

	return move(result);
}
