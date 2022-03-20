#include "Error.hpp"

Error::Error() {}

Error::Error(string errorMessage, string pointOfError):errorMessage(errorMessage),pointOfError(pointOfError)
{
}

string Error::GetErrorMessage() const
{
	return errorMessage;
}

string Error::GetPointOfError() const
{
	return pointOfError;
}

