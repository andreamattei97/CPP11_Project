#ifndef USER_MANAGER_HPP
#define USER_MANAGER_HPP

#include "UserDatabase.hpp"
#include "ApplicationInterface.hpp"

class UserManager
{
public:
	UserManager(ApplicationInterface& applicationInterface, UserDatabase& database);

	void ManageUsers();

	void SaveDatabase();
private:
	regex lineRegex = regex("([[:alnum:]]|/|[[:space:]])+"); //the regex for checking if a string has only valid characters (letters, numbers e spaces)
	ApplicationInterface& applicationInterface;
	UserDatabase& database;

	void AddUser();
	void RemoveUser();
	void ModifyUser();
	void VisualizeUser();
	void VisualizeAllUsers(); //visualizes the id, the username and the type of every users


	void ModifySimpleUser(int id);
	void ModifyCompanyUser(int id);
	void ModifyGroupUser(int id);
};

#endif


