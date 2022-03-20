#include "GroupUser.hpp"

string GroupUser::GetAddress() const
{
	return address;
}

bool GroupUser::SetAddress(const string & value)
{
	address = value;
	addressInitialized = true;
	return true;
}

bool GroupUser::AddressInitialized() const
{
	return addressInitialized;
}

string GroupUser::GetAreaOfOperation() const
{
	return areaOfOperation;
}

bool GroupUser::SetAreaOfOperation(const string & value)
{
	areaOfOperation = value;
	areaOfOperationInitialized = true;
	return true;
}

bool GroupUser::AreaOfOperationInitialized() const
{
	return areaOfOperationInitialized;
}

GroupUser::GroupUser(int id):User(Group,id)
{
}

void GroupUser::PrintAdditionalParameter(ostream & os) const
{
	if(addressInitialized)
		os << ",address:" << address;
	if(areaOfOperationInitialized)
		os << ",areaOfOperation:" << areaOfOperation;
}

string GroupUser::GetUsername() const
{
	return GetName();
}

PropertyHandler<string> GroupUser::GetAttributeHandler(string attribute)
{
	PropertyHandler<string> result;
	if (attribute == "address")
	{
		result = PropertyHandler<string>(bind(&GroupUser::SetAddress, &(*this), placeholders::_1),
			bind(&GroupUser::GetAddress, &(*this)), bind(&GroupUser::AddressInitialized, &(*this)));
	}
	else if (attribute == "areaOfOperation")
	{
		result = PropertyHandler<string>(bind(&GroupUser::SetAreaOfOperation, &(*this), placeholders::_1),
			bind(&GroupUser::GetAreaOfOperation, &(*this)), bind(&GroupUser::AreaOfOperationInitialized, &(*this)));
	}
	return result;
}
