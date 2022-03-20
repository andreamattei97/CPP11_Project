#ifndef SIMPLE_USER_H
#define SIMPLE_USER_H

#include "User.hpp"

class SimpleUser:public User
{
public:
	SimpleUser(int id);

	string GetSurname() const;
	bool SetSurname(const string& value);
	bool SurnameInitialized() const;

	string GetAddress() const;
	bool SetAddress(const string& value);
	bool AddressInitialized() const;

	virtual PropertyHandler<string> GetAttributeHandler(string attribute) override;
	virtual string GetUsername() const override;


protected:
	void PrintAdditionalParameter(ostream& os) const;

private:
	string surname;
	bool surnameInitialized = false;

	string address;
	bool addressInitialized = false;

};
#endif

