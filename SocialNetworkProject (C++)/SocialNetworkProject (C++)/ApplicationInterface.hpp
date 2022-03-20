#ifndef APPLICATION_INTERFACE_HPP
#define APPLICATION_INTERFACE_HPP

#include <string>
#include <iostream>
#include <functional>

#include "Date.hpp"
#include "Error.hpp"
#include "PropertyHandler.hpp"
#include "User.hpp"
#include "SimpleUser.hpp"
#include "CompanyUser.hpp"
#include "GroupUser.hpp"
#include "News.hpp"

using namespace std;

class ApplicationInterface
{
public:
	
	bool intInput(int &buffer,const string& help = "",bool repeat=false, const function<bool(int)>& condition = [](int i){ return true; }) const;
	bool floatInput(float &buffer, const string& help = "", bool repeat = false, const function<bool(float)>& condition = [](float i) { return true; }) const;
	bool lineInput(string& buffer, const string& help = "",bool repeat = false, const function<bool(string)>& condition = [](string i) { return true; }) const;
	bool dateInput(Date& buffer, const string& help = "",bool repeat = false, const function<bool(string)>& condition = [](string i) { return true; }) const;
	bool QuestionInput(bool& result, const string& help = "", bool repeat=false) const;
	bool tryAgainInput()const;
	void PrintError(const Error& error)const;
	void PrintMessage(const string& message,int tabCharacters=0)const;
	void PrintUser(const User& user)const;
	void PrintNews(const News& news, string userName, int tabCharacters=0,int lineNumber=-1)const;
	void PrintStatistic(const string& message, float number, int precision = 1) const;
	template <typename T>
	void PrintAttribute(const PropertyHandler<T>& handler)const;
	
private:

	string invalidInput = "Input non valido";
	string intInputMessage = "Inserisci un numero intero";
	string dateInputMessage = "Inserisci la data";
	string wordInputMessage = "Inserisci una parola (attenzione tutte le parole dopo la prima saranno ignorate)";
	string tryAgainMessage = "Per riprovare ad inserire l'input preme Y (per qualsiasi altro tasto l'input viene interrotto)";
	string questionInputMessage = "Inserisci Y per si o N per no";

};

template<typename T>
inline void ApplicationInterface::PrintAttribute(const PropertyHandler<T>& handler) const
{
	if (handler.IsInitialized())
	{
		cout << "Valore corrente dell'attributo: " << handler.Get() <<endl;
	}
	else
	{
		cout << "Attributo non inizializzato" << endl;
	}
}

#endif 
