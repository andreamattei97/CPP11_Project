#include "User.hpp"

string User::GetName() const
{
	return name;
}

bool User::SetName(const string & value)
{
	name = value;
	nameInitiliazed = true;
	return true;
}

bool User::NameInitialized() const
{
	return nameInitiliazed;
}

PropertyHandler<string> User::GetNameHandler()
{
	return PropertyHandler<string>(bind(&User::SetName, &(*this), placeholders::_1), bind(&User::GetName, &(*this)), bind(&User::BirthdateInitialized, &(*this)));
}

Date User::GetBirthdate() const
{
	return birthdate;
}

bool User::SetBirthdate(const Date & value)
{
	if (!value.valid())
		return false;
	birthdateInitiliazed = true;
	birthdate = value;
	return true;
}

bool User::BirthdateInitialized() const
{
	return birthdateInitiliazed;
}

PropertyHandler<Date> User::GetBirthdateHandler()
{
	return PropertyHandler<Date>(bind(&User::SetBirthdate,&(*this),placeholders::_1),bind(&User::GetBirthdate,&(*this)),bind(&User::BirthdateInitialized,&(*this)));
}

User::User(UserType type,int userId):userType(type),id(userId)
{
}

void User::PrintAdditionalParameter(ostream & os) const
{
}

ostream & operator<<(ostream & os, const User & u)
{
	os << u.id << ',';
	switch (u.userType) 
	{
	case Simple:
		os << "s,";
		break;
	case Company:
		os << "c,";
		break;
	case Group:
		os << "g,";
		break;
	}
	os << "{" << "name:" << u.name << ",birthdate:" << u.birthdate;
	u.PrintAdditionalParameter(os);
	os << "}";
	return os;
}
