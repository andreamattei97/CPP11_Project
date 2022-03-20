#include "UserDatabase.hpp"



UserDatabase::UserDatabase(const string& usersFilePath):file(usersFilePath)
{
}

unique_ptr<Error> UserDatabase::LoadDatabase()
{
	list<string> users;
	unique_ptr<Error> error = file.ExtractLines(users);
	list<string>::iterator iter = users.begin();
	for (; error==nullptr && iter != users.end(); iter++) 
	{
			unique_ptr<User> newUser = extractor.ExtractUser(*iter);
			if (newUser != nullptr) 
			{
				error = AddUser(move(newUser));
			}
			else
			{
				error = extractor.GetError();
			}
	}

	return error;
}

unique_ptr<Error> UserDatabase::SaveDatabase()
{
	return file.WriteMapFilePtr(database.begin(), database.end());
}

unique_ptr<Error> UserDatabase::AddUser(unique_ptr<User> newUser)
{
	unique_ptr<Error> error;
	int userId = newUser->id;

	if (userId < 0)
	{
		error = make_unique<Error>(Error(invalidId, to_string(userId)));
	}

	if (ContainsId(userId))
	{
		error = make_unique<Error>(Error(duplicatedUser, to_string(userId)));

	}

	if (error==nullptr && newUser->userType == NotValid)
	{
		error = make_unique<Error>(Error(errorUserType, to_string(userId)));
	}

	if (error == nullptr && !newUser->NameInitialized())
	{
		error = make_unique<Error>(Error(missingParameter, "nome"));
	}

	if (error == nullptr && !newUser->BirthdateInitialized())
	{
		error = make_unique<Error>(Error(missingParameter, "data di nascita"));
	}

	if (error == nullptr)
	{
		database.insert(make_pair(userId, move(newUser)));
		if (mAddCallback != nullptr)
		{
			error = mAddCallback(*database[userId]);
		}
	}
	return error;
}

unique_ptr<Error> UserDatabase::RemoveUser(int id)
{
	unique_ptr<Error> error;
	if (!ContainsId(id))
	{
		error = make_unique<Error>(Error(userNotFound, to_string(id)));
	}
	else
	{
		error = mRemoveCallback(*database[id]);
		database.erase(id);
	}
	return error;
}

PropertyHandler<Date> UserDatabase::GetBirthdate(int id)
{
	if(!ContainsId(id))
		return PropertyHandler<Date>();
	return database[id]->GetBirthdateHandler();
}

PropertyHandler<string> UserDatabase::GetName(int id)
{
	if (!ContainsId(id))
		return PropertyHandler<string>();
	return database[id]->GetNameHandler();
}

PropertyHandler<string> UserDatabase::GetSecondaryAttribute(int id, string attribute)
{
	if (!ContainsId(id))
		return PropertyHandler<string>();
	return database[id]->GetAttributeHandler(attribute);
}

void UserDatabase::VisitAllUsers(function<void(const User&)> visitFunction) const
{
	for (auto& user : database)
	{
		visitFunction(*(user.second));
	}
}



bool UserDatabase::ContainsId(int id) const
{
	if (database.find(id) != database.end())
		return true;
	else
		return false;
}


void UserDatabase::SetAddCallback(callback<User> callback)
{
	mAddCallback = callback;
}

void UserDatabase::SetRemoveCallback(callback<User> callback)
{
	mRemoveCallback = callback;
}

const User & UserDatabase::operator[](const int id) const
{
	auto iter = database.find(id);
	if (iter != database.end())
	{
		return *(*iter).second;
	}
	return errorResult;
}

UserDatabase::ErrorUser::ErrorUser():User(NotValid,-1)
{
}

PropertyHandler<string> UserDatabase::ErrorUser::GetAttributeHandler(string attribute)
{
	return PropertyHandler<string>();
}

string UserDatabase::ErrorUser::GetUsername() const
{
	return string();
}
