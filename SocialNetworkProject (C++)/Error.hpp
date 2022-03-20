#ifndef ERROR_H
#define ERROR_H

#include <string>

using namespace std;

class Error
{
public:
	Error();
	Error(string errorMessage, string pointOfError = "");

	string GetErrorMessage() const;
	string GetPointOfError() const;

private:
	string errorMessage;
	string pointOfError;

};

#endif 

