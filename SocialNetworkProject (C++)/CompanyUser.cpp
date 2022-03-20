#include "CompanyUser.hpp"

string CompanyUser::GetRegisteredOffice() const
{
	return registeredOffice;
}

bool CompanyUser::SetRegisteredOffice(const string & value)
{
	registeredOffice = value;
	registeredOfficeInitialized = true;
	return true;
}

bool CompanyUser::RegisteredOfficeInitialized() const
{
	return registeredOfficeInitialized;
}

string CompanyUser::GetHeadquarter() const
{
	return headquarter;
}

bool CompanyUser::SetHeadquarter(const string & value)
{
	headquarter = value;
	headquarterInitialized = true;
	return true;
}

bool CompanyUser::HeadquarterInitialized() const
{
	return headquarterInitialized;
}

string CompanyUser::GetFieldOfInterest() const
{
	return fieldOfInterest;
}

bool CompanyUser::SetFieldOfInterest(const string & value)
{
	fieldOfInterest = value;
	fieldOfInterestInitialized = true;
	return true;
}

bool CompanyUser::FieldOfInterestInitialized() const
{
	return fieldOfInterestInitialized;
}

CompanyUser::CompanyUser(int id):User(Company,id)
{
}

void CompanyUser::PrintAdditionalParameter(ostream & os) const
{
	if(registeredOfficeInitialized)
		os << ",registeredOffice:" << registeredOffice;
	if(headquarterInitialized)
		os << ",headquarter:" << headquarter;
	if (fieldOfInterestInitialized)
		os << ",fieldOfInterest:" << fieldOfInterest;
}

string CompanyUser::GetUsername() const
{
	return GetName();
}

PropertyHandler<string> CompanyUser::GetAttributeHandler(string attribute)
{
	PropertyHandler<string> result;
	if (attribute == "fieldOfInterest")
	{
		result = PropertyHandler<string>(bind(&CompanyUser::SetFieldOfInterest, &(*this), placeholders::_1), 
			bind(&CompanyUser::GetFieldOfInterest, &(*this)), bind(&CompanyUser::FieldOfInterestInitialized, &(*this)));
	}
	else if (attribute == "headquarter")
	{
		result = PropertyHandler<string>(bind(&CompanyUser::SetHeadquarter, &(*this), placeholders::_1),
			bind(&CompanyUser::GetHeadquarter, &(*this)), bind(&CompanyUser::HeadquarterInitialized, &(*this)));
	}
	else if (attribute == "registeredOffice") 
	{
		result = PropertyHandler<string>(bind(&CompanyUser::SetRegisteredOffice, &(*this), placeholders::_1),
			bind(&CompanyUser::GetRegisteredOffice, &(*this)), bind(&CompanyUser::RegisteredOfficeInitialized, &(*this)));
	}
	return result;
}
