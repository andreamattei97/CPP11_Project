#include "SimpleUser.hpp"



string SimpleUser::GetSurname() const
{
	return surname;
}

bool SimpleUser::SetSurname(const string & value)
{
	surname = value;
	surnameInitialized = true;
	return true;
}

bool SimpleUser::SurnameInitialized() const
{
	return surnameInitialized;
}

string SimpleUser::GetAddress() const
{
	return address;
}

bool SimpleUser::SetAddress(const string & value)
{
	address = value;
	addressInitialized = true;
	return true;
}

bool SimpleUser::AddressInitialized() const
{
	return addressInitialized;
}

SimpleUser::SimpleUser(int id):User(Simple,id)
{

}

void SimpleUser::PrintAdditionalParameter(ostream & os) const
{
	if(surnameInitialized)
		os << ",surname:" << surname;
	if(addressInitialized)
		os << ",address:" << address;
}

string SimpleUser::GetUsername() const
{
	string userName = GetName();
	if (SurnameInitialized())
		userName += " " + GetSurname();
	return userName;
}

PropertyHandler<string> SimpleUser::GetAttributeHandler(string attribute)
{
	PropertyHandler<string> result;
	if (attribute == "surname")
	{
		result = PropertyHandler<string>(bind(&SimpleUser::SetSurname, &(*this), placeholders::_1),
			bind(&SimpleUser::GetSurname, &(*this)), bind(&SimpleUser::SurnameInitialized, &(*this)));
	}
	else if (attribute == "address")
	{

		result = PropertyHandler<string>(bind(&SimpleUser::SetAddress, &(*this), placeholders::_1),
			bind(&SimpleUser::GetAddress, &(*this)), bind(&SimpleUser::AddressInitialized, &(*this)));
	}
	return result;
}
