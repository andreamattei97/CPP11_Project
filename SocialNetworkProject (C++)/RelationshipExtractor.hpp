#ifndef RELATIONSHIP_EXTRACTOR_H
#define RELATIONSHIP_EXTRACTOR_H

#include <memory>
#include <regex>
#include <string>

#include "Error.hpp"
#include "Relationships.hpp"

class RelationshipExtractor
{
public:
	unique_ptr<Error> ExtractRelationship(const string& line, int& owner, int& target, RelationshipType& type);

private:
	regex relationshipRegex=regex ("[[:digit:]]+,[[:digit:]]+,(parent|son|spouse|friend|acquaintance|membership|member|associate|employee|employer)");
	regex numberRegex = regex("[[:digit:]]+");
	regex typeRegex = regex("(parent|son|spouse|friend|acquaintance|membership|member|associate|employee|employer)");

	string invalidRelationshipSyntax = "formato relazione non valido";
};

#endif

