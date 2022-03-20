#include "UserManager.hpp"

UserManager::UserManager(ApplicationInterface &applicationInterface, UserDatabase & database) :applicationInterface(applicationInterface), database(database)
{
}

void UserManager::AddUser()
{
	unique_ptr<User> result;
	int id, type;
	string name;
	Date birthdate;
	bool aborted = !applicationInterface.intInput(id, "Inserisci l'ID utente", true, [this](int id) {return !database.ContainsId(id) && id>=0; });

	if (!aborted)
	{
		aborted = !applicationInterface.intInput(type, "Inserire tipo utente: 0=Semplice, 1=Azienda, 2=Gruppo", true, [](int i) {return i<3 && i>-1; });
	}

	if (!aborted)
	{
		aborted = !applicationInterface.lineInput(name, "Inserire il nome dell'utente", true, [this](string s) {return regex_match(s, lineRegex); });
	}
	if (!aborted) 
	{
		aborted = !applicationInterface.dateInput(birthdate, "Inserire la data di nascita/creazione dell'utente", true);
	}

	if (!aborted)
	{
		switch (type)
		{
		case 0:
			result = make_unique<SimpleUser>(SimpleUser(id));
			break;
		case 1:
			result = make_unique<CompanyUser>(CompanyUser(id));
			break;
		case 2:
			result = make_unique<GroupUser>(GroupUser(id));
			break;
		}

		result->SetName(name);
		result->SetBirthdate(birthdate);

		database.AddUser(move(result));
		cout << "Utente inserito con successo" << endl;
	}
	else
	{
		cout << "Errore nell'inserimento dell'utente" << endl;
	}
}

void UserManager::RemoveUser()
{
	int id;
	bool aborted = !applicationInterface.intInput(id, "Inserisci l'ID utente", true, [this](int id) {return database.ContainsId(id); });
	
	if (!aborted)
	{
		unique_ptr<Error> error=database.RemoveUser(id);
		if (error != nullptr)
		{
			applicationInterface.PrintError(*error);
		}
		else
		{
			applicationInterface.PrintMessage("Utente eliminato con successo");
		}
	}
}

void UserManager::ModifyUser()
{
	int id;
	bool aborted = !applicationInterface.intInput(id, "Inserisci l'ID utente", true, [this](int id) {return database.ContainsId(id); });
	
	if (!aborted)
	{
		switch (database[id].userType)
		{
		case Simple:
			ModifySimpleUser(id);
			break;
		case Company:
			ModifyCompanyUser(id);
			break;
		case Group:
			ModifyGroupUser(id);
			break;
		}
	}
}

void UserManager::VisualizeUser()
{
	int id;
	bool aborted = !applicationInterface.intInput(id, "Inserisci l'id dell'utente da visualizzare", true, [this](int id) {return database.ContainsId(id); });
	if (!aborted)
	{
		applicationInterface.PrintUser(database[id]);
	}
}

void UserManager::VisualizeAllUsers()
{
	database.VisitAllUsers([this](const User& user) {
		string message = user.GetUsername()+ " id: "+to_string(user.id) + " tipo: ";
		switch (user.userType)
		{
		case Simple:
			message += "semplice";
			break;
		case Company:
			message += "azienda";
			break;
		case Group:
			message += "gruppo";
			break;
		}
		applicationInterface.PrintMessage(message);
	});
}

void UserManager::SaveDatabase()
{
	unique_ptr<Error> error=database.SaveDatabase();
	if (error != nullptr)
	{
		applicationInterface.PrintError(*error);
	}
	else
	{
		applicationInterface.PrintMessage("Database utenti salvato");
	}
}

void UserManager::ModifySimpleUser(int id)
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Inserisci l'attributo che vuoi cambiare: 0=nome, 1=cognome, 2=data di nascita, 3=indirizzo di casa, 4=esci da mofica utente ", true, [](int option) {return option >= 0 && option < 5; });

		if (!aborted)
		{
			if (option == 1 || option == 0 || option == 3)
			{
				PropertyHandler<string> handler;
				switch (option)
				{
				case 0:
					handler = database.GetName(id);
					break;
				case 1:
					handler = database.GetSecondaryAttribute(id, "surname");
					break;
				case 3:
					handler = database.GetSecondaryAttribute(id, "address");
					break;
				}
				string input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.lineInput(input, "Inserisci il nuovo valore dell'attributo", true, [this](string s) {return regex_match(s, lineRegex); });
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else if(option == 2)
			{
				PropertyHandler<Date> handler = database.GetBirthdate(id);
				Date input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.dateInput(input, "Inserisci il nuovo valore dell'attributo", true);
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else
			{
				finished = true;
			}
		}
	}
}

void UserManager::ModifyCompanyUser(int id)
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Inserisci l'attributo che vuoi cambiare: 0=nome, 1=sede legale, 2=data di nascita, 3=quartier generale, 4=area di competenza, 5=esci da mofica utente ", true, [](int option) {return option >= 0 && option < 6; });

		if (!aborted)
		{
			if (option == 1 || option == 0 || option == 3 || option == 4 )
			{
				PropertyHandler<string> handler;
				switch (option)
				{
				case 0:
					handler = database.GetName(id);
					break;
				case 1:
					handler = database.GetSecondaryAttribute(id, "registeredOffice");
					break;
				case 3:
					handler = database.GetSecondaryAttribute(id, "headquarter");
					break;
				case 4:
					handler = database.GetSecondaryAttribute(id, "fieldOfInterest");
					break;
				}
				string input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.lineInput(input, "Inserisci il nuovo valore dell'attributo", true, [this](string s) {return regex_match(s, lineRegex); });
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else if (option == 2)
			{
				PropertyHandler<Date> handler = database.GetBirthdate(id);
				Date input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.dateInput(input, "Inserisci il nuovo valore dell'attributo", true);
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else
			{
				finished = true;
			}
		}
	}
}

void UserManager::ModifyGroupUser(int id)
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Inserisci l'attributo che vuoi cambiare: 0=nome, 1=area di operazione, 2=data di nascita, 3=indirizzo sede, 4=esci da mofica utente ", true, [](int option) {return option >= 0 && option < 5; });

		if (!aborted)
		{
			if (option == 1 || option == 0 || option == 3)
			{
				PropertyHandler<string> handler;
				switch (option)
				{
				case 0:
					handler = database.GetName(id);
					break;
				case 1:
					handler = database.GetSecondaryAttribute(id, "areaOfOperation");
					break;
				case 3:
					handler = database.GetSecondaryAttribute(id, "address");
					break;
				}
				string input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.lineInput(input, "Inserisci il nuovo valore dell'attributo", true, [this](string s) {return regex_match(s, lineRegex); });
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else if (option == 2)
			{
				PropertyHandler<Date> handler = database.GetBirthdate(id);
				Date input;
				applicationInterface.PrintAttribute(handler);
				aborted = !applicationInterface.dateInput(input, "Inserisci il nuovo valore dell'attributo", true);
				if (!aborted)
				{
					handler.Set(input);
					applicationInterface.PrintMessage("Attributo modificato con successo");
				}
			}
			else
			{
				finished = true;
			}
		}
	}
}

void UserManager::ManageUsers()
{

	bool finished = false;
	while (!finished)
	{
		int option;
		if (applicationInterface.intInput(option, "Comandi: 0=Inserisci utente, 1=Elimina utente, 2=Modifica utente, 3=Visualizza utente, 4=Visualizza tutti gli utenti, 5=Esci da gestione utenti",false, [](int i) {return i >= 0 && i < 6; }))
		{
			switch (option)
			{
			case 0:
				AddUser();
				break;
			case 1:
				RemoveUser();
				break;
			case 2:
				ModifyUser();
				break;
			case 3:
				VisualizeUser();
				break;
			case 4:
				VisualizeAllUsers();
				break;
			case 5:
				finished = true;
				break;
			}
		}
	}
}
