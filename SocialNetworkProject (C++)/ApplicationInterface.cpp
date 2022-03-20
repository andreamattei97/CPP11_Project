#include "ApplicationInterface.hpp"

#include <iomanip>

bool ApplicationInterface::intInput(int& buffer,const string & help,bool repeat, const function<bool(int)>& condition) const
{
	bool success = false;
	bool running = true;
	while (running) 
	{
		if (help != "")
			cout << help << endl;
		else
			cout << intInputMessage << endl;

		cin >> buffer;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (repeat) 
			{
				running = tryAgainInput();
			}
			else 
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else if (!(condition(buffer)))
		{
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else
		{
			running = false;
			success = true;
		}

	}
	return success;
}

bool ApplicationInterface::floatInput(float & buffer, const string & help, bool repeat, const function<bool(float)>& condition) const
{
	bool success = false;
	bool running = true;
	while (running)
	{
		if (help != "")
			cout << help << endl;
		else
			cout << intInputMessage << endl;

		cin >> buffer;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else if (!(condition(buffer)))
		{
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else
		{
			running = false;
			success = true;
		}

	}
	return success;
}

bool ApplicationInterface::lineInput(string& buffer,const string & help,bool repeat, const function<bool(string)>& condition) const
{
	bool success = false;
	bool running = true;
	while (running)
	{
		if (help != "")
			cout << help << endl;
		else
			cout << intInputMessage << endl;

		getline(cin, buffer);

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else if (!(condition(buffer)))
		{
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
		else
		{
			running = false;
			success = true;
		}

	}
	return success;
}

bool ApplicationInterface::dateInput(Date & buffer, const string & help,bool repeat, const function<bool(string)>& condition) const
{
	bool success = false;
	bool running = true;
	while (running) 
	{
		int day, month, year;
		bool error = false;

		if (help != "")
			cout << help << endl;
		else
			cout << dateInputMessage << endl;

		if (!intInput(day, "Inserisci il giorno"))
		{
			error = true;
		}

		if (!error && !intInput(month, "Inserisci il mese"))
		{
			error = true;
		}

		if (!error && !intInput(year, "Inserisci l'anno"))
		{
			error = true;
		}

		if (!error)
		{
			buffer.set_day(day);
			buffer.set_month(month);
			buffer.set_year(year);
		}

		if (!buffer.valid())
		{
			error = true;
		}

		if (!error)
		{
			running = false;
			success = true;
		}
		else 
		{
			if (repeat)
			{
				running = tryAgainInput();
			}
			else
			{
				PrintMessage(invalidInput);
				running = false;
			}
		}
	}
	return success;
}

bool ApplicationInterface::QuestionInput(bool& result,const string & help, bool repeat) const
{

	bool continuing = true;
	bool success = false;
	while (continuing)
	{
		if (help != "")
			cout << help << endl;
		else
			cout << questionInputMessage;

		char input;
		cin >> input;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (input == 'Y')
		{
			result = true;
			success = true;
			continuing = false;
		}
		else if (input == 'N')
		{
			result = false;
			success = true;
			continuing = false;
		}
		else
		{
			if (repeat)
			{
				continuing = tryAgainInput();
				if (!continuing)
					success = false;
			}
			else
			{
				PrintMessage(invalidInput);
				success = false;
				continuing = false;
			}
		}
	}
	return success;
}

bool ApplicationInterface::tryAgainInput() const
{
	char input;
	cout << invalidInput << endl ;
	cout << tryAgainMessage <<endl;
	cin >> input;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (input == 89)
		return true;
	else
		return false;
}

void ApplicationInterface::PrintError(const Error & error) const
{
	cerr << "Error: " << error.GetErrorMessage() << endl;
	if(error.GetPointOfError()!="")
		cerr << "Punto di errore: " << error.GetPointOfError() << endl;
}

void ApplicationInterface::PrintMessage(const string & message,int tabCharacters) const
{
	for (int i = 0; i < tabCharacters; i++)
		cout << "\t";
	cout << message<<endl;
}

void ApplicationInterface::PrintUser(const User & user) const
{
	switch (user.userType)
	{
	case Simple:
		cout << "Nome: " << user.GetName() <<endl;
		if (((const SimpleUser&)user).SurnameInitialized())
			cout << "Cognome: " << ((const SimpleUser&)user).GetSurname() <<endl;
		cout << "Data di nascita: " << user.GetBirthdate() << endl;
		if (((const SimpleUser&)user).AddressInitialized())
			cout << "Indirizzo: " << ((const SimpleUser&)user).GetAddress()<<endl;
		break;
	case Company:
		cout << "Nome: " << user.GetName()<<endl;
		cout << "Data di creazione: " << user.GetBirthdate() << endl;
		if (((const CompanyUser&)user).RegisteredOfficeInitialized())
			cout << "Sede legale: " << ((const CompanyUser&)user).GetRegisteredOffice() <<endl;
		if (((const CompanyUser&)user).HeadquarterInitialized())
			cout << "Quartier generale: " << ((const CompanyUser&)user).GetHeadquarter() <<endl;
		if (((const CompanyUser&)user).FieldOfInterestInitialized())
			cout << "Area di competenza: " << ((const CompanyUser&)user).GetFieldOfInterest() <<endl;
		break;
	case Group:
		cout << "Nome: " << user.GetName() << endl;
		cout << "Data di creazione: " << user.GetBirthdate() << endl;
		if (((const GroupUser&)user).AreaOfOperationInitialized())
			cout << "Area di opeazione: " << ((const GroupUser&)user).GetAreaOfOperation() << endl;
		if (((const GroupUser&)user).AddressInitialized())
			cout << "Indirizzo sede: " << ((const GroupUser&)user).GetAddress() << endl;
		break;
	default:
		cout << "Utente non valido";
		break;
	}
}

void ApplicationInterface::PrintNews(const News & news,string userName,int tabCharacters, int lineNumber) const
{
	for (int i = 0; i < tabCharacters; i++)
		cout << "\t";
	if (lineNumber >= 0)
		cout << lineNumber << ")";
	cout << userName<<": " << news.GetMessage() << " scritto il: " << news.date <<endl;
}

void ApplicationInterface::PrintStatistic(const string & message, float number, int precision) const
{
	cout << setprecision(precision) << fixed;
	cout << message << ": " << number << endl;
}


