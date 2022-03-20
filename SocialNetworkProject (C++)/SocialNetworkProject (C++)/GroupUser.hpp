#ifndef GROUP_USER_H
#define GROUP_USER_H

#include "User.hpp"

class GroupUser :
	public User
{
public:
	GroupUser(int id);

	string GetAddress() const;
	bool SetAddress(const string& value);
	bool AddressInitialized() const;

	string GetAreaOfOperation() const;
	bool SetAreaOfOperation(const string& value);
	bool AreaOfOperationInitialized() const;

	virtual PropertyHandler<string> GetAttributeHandler(string attribute) override;	
	virtual string GetUsername() const override;

protected:
	void PrintAdditionalParameter(ostream& os) const;

private:
	string address;
	bool addressInitialized = false;

	string areaOfOperation;
	bool areaOfOperationInitialized = false;

};

#endif

