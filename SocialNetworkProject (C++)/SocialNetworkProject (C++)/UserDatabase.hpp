#ifndef USER_DATABASE_HPP
#define USER_DATABASE_HPP

#include <memory>
#include <map>
#include <functional>

#include "FileManager.hpp"
#include "User.hpp"
#include "Extractor.hpp"
#include "RelationshipExtractor.hpp"
#include "Error.hpp"
#include "PropertyHandler.hpp"
#include "Date.hpp"


template <typename T>
using callback = function<unique_ptr<Error>(const T&)>; //alias for callback functions (called when an user is instered or removed)

class UserDatabase
{
public:

	//the token class for invalid index
	class ErrorUser :public User
	{
	public:
		ErrorUser();
		virtual PropertyHandler<string> GetAttributeHandler(string attribute) override;
		virtual string GetUsername() const override;
	};

	UserDatabase(const string& usersFilePath);

	unique_ptr<Error> LoadDatabase();
	unique_ptr<Error> SaveDatabase();

	unique_ptr<Error> AddUser(unique_ptr<User> newUser);
	unique_ptr<Error> RemoveUser(int id);

	//access to an attribute of an user
	PropertyHandler<Date> GetBirthdate(int id);
	PropertyHandler<string> GetName(int id);
	PropertyHandler<string> GetSecondaryAttribute(int id,string attribute);

	void VisitAllUsers(function<void(const User&)> visitFunction) const;

	bool ContainsId(int id) const;

	void SetAddCallback(callback<User> callback);
	void SetRemoveCallback(callback<User> callback);

	const User& operator[](const int id) const;

private:

	string duplicatedUser = "Utente duplicato";
	string notValidUser = "Utente non valido";
	string errorUserType = "Tipo utente non permesso";
	string userNotFound = "Utente non trovato";
	string missingParameter = "parametro mancante";
	string invalidId = "Id non valido (id negativo)";

	const ErrorUser errorResult = ErrorUser(); //the invalid result returned in case of an invalid index

	map<int, unique_ptr<User>> database;
	FileManager file;
	Extractor extractor;
	callback<User> mAddCallback;
	callback<User> mRemoveCallback;
};

#endif

