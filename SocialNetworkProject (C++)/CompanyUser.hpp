#ifndef COMPANY_USER_H
#define COMPANY_USER_H

#include "User.hpp"

class CompanyUser :
	public User
{
public:
	CompanyUser(int id);

	string GetRegisteredOffice() const;
	bool SetRegisteredOffice(const string& value);
	bool RegisteredOfficeInitialized() const;

	string GetHeadquarter() const;
	bool SetHeadquarter(const string& value);
	bool HeadquarterInitialized() const;

	string GetFieldOfInterest() const;
	bool SetFieldOfInterest(const string& value);
	bool FieldOfInterestInitialized() const;

	virtual PropertyHandler<string> GetAttributeHandler(string attribute) override;
	virtual string GetUsername() const override;

protected:
	void PrintAdditionalParameter(ostream& os) const;

private:
	string registeredOffice;
	bool registeredOfficeInitialized = false;

	string headquarter;
	bool headquarterInitialized = false;

	string fieldOfInterest;
	bool fieldOfInterestInitialized = false;

};

#endif

