#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

#include "Date.hpp"
#include "PropertyHandler.hpp"

using namespace std;

enum UserType {
	NotValid=-1,Simple,Company,Group
};

class User
{
public:
	const int id;
	const UserType userType;

	User(UserType type, int id);

	string GetName() const;
	bool SetName(const string& value);
	bool NameInitialized() const;
	PropertyHandler<string> GetNameHandler();


	Date GetBirthdate() const;
	bool SetBirthdate(const Date& value);
	bool BirthdateInitialized() const;
	PropertyHandler<Date> GetBirthdateHandler();

	virtual PropertyHandler<string> GetAttributeHandler(string attribute) = 0;
	virtual string GetUsername() const = 0;

	friend ostream & operator<<(ostream & os, const User& u);

protected:
	virtual void PrintAdditionalParameter(ostream& os) const;

private:
	string name;
	bool nameInitiliazed = false;
	Date birthdate;
	bool birthdateInitiliazed = false;

};

//friend operator
ostream & operator<<(ostream & os, const User& u);

#endif
