#include "RelationshipExtractor.hpp"

unique_ptr<Error> RelationshipExtractor::ExtractRelationship(const string& line, int & owner, int & target, RelationshipType & type)
{
	unique_ptr<Error> error;
	if (!regex_match(line, relationshipRegex))
	{
		error = make_unique<Error>(Error(invalidRelationshipSyntax, line));
	}
	if (error == nullptr)
	{
		sregex_iterator iter(line.begin(), line.end(), numberRegex);
		owner = stoi((*iter)[0]); // the first number is the id of the owner of the relationship
		iter++;
		target = stoi((*iter)[0]); // the second number is the id of the target of the relationship
		smatch typeResult;
		regex_search(line, typeResult, typeRegex); //find the type of the relationship (saved as a string)
		string typeString = typeResult.str();
		if (typeString == "parent")
		{
			type = Parent;
		}
		else if (typeString == "son")
		{
			type = Son;
		}
		else if (typeString == "spouse")
		{
			type = Spouse;
		}
		else if (typeString == "friend")
		{
			type = Friend;
		}
		else if (typeString == "member")
		{
			type = Member;
		}
		else if (typeString == "membership")
		{
			type = Membership;
		}
		else if (typeString == "associate")
		{
			type = Associate;
		}
		else if (typeString == "employee")
		{
			type = Employee;
		}
		else if (typeString == "employer")
		{
			type = Employer;
		}
		else
		{
			type = Acquaintance;
		}
	}
	return error;
}
