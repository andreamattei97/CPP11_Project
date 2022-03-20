#include "RelationshipDatabase.hpp"


RelationshipDatabase::RelationshipDatabase(const string& relationshipFilePath,condition<int> validId):file(relationshipFilePath),mValidId(validId),
explorer(bind(&RelationshipDatabase::operator[],this,placeholders::_1))
{
}

unique_ptr<Error> RelationshipDatabase::LoadDatabase()
{
	list<string> relationships;
	unique_ptr<Error> error = file.ExtractLines(relationships);
	list<string>::iterator iter = relationships.begin();
	for (; error == nullptr && iter != relationships.end(); iter++)
	{
		RelationshipData newData;
		error = relationshipExtractor.ExtractRelationship(*iter, newData.owner, newData.target, newData.type);

		if (error == nullptr)
		{
			error=AddRelationship(newData);
			if (error != nullptr) //add the line to the point of error in case of error
			{
				error = make_unique<Error>(Error(error->GetErrorMessage(), error->GetPointOfError() + " \nLinea: " + *iter));
			}
		}
	}

	return error;
}

unique_ptr<Error> RelationshipDatabase::SaveDatabase()
{
	list<RelationshipData> data;
	for (const auto& relationships : database)
	{
		relationships.second->GetAllRelationships(data);
	}
	return file.WriteListFile(data.begin(),data.end());
}

unique_ptr<Error> RelationshipDatabase::AddRelationship(RelationshipData data)
{

	unique_ptr<Error> error;

	if (!mValidId(data.owner) || database.find(data.owner) == database.end())
	{
		error = make_unique<Error>(Error(invalidId,"ID:" + to_string(data.owner)));
	}

	if (!mValidId(data.target) || database.find(data.target) == database.end())
	{
		error = make_unique<Error>(Error(invalidId, "ID:" + to_string(data.target)));
	}

	if (data.owner == data.target)
	{
		error = make_unique<Error>(Error(invalidRelationship, "ID1:" + to_string(data.owner) + "ID2:" + to_string(data.target)));
	}
	
	if (error == nullptr)
	{
		switch (data.type) //check if the type of the users matches and call the function associate with the type of the relationship
		{
		case Parent:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
			{
				error = AddParenthoodRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Son:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
			{
				error = AddParenthoodRelationship(data.target, data.owner);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Spouse:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
			{
				error = AddMarriageRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Friend:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
			{
				error = AddFriendshipRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Acquaintance:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
			{
				error = AddAcquaintanceshipRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Employer:
			if (database[data.owner]->ownerType == Company && database[data.target]->ownerType == Simple)
			{
				error = AddEmploymentRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Company ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Employee:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Company)
			{
				error = AddEmploymentRelationship(data.target, data.owner);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Membership:
			if (database[data.owner]->ownerType == Group && database[data.target]->ownerType == Simple)
			{
				error = AddMembershipRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Group ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Member:
			if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Group)
			{
				error = AddMembershipRelationship(data.target, data.owner);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		case Associate:
			if(database[data.owner]->ownerType==Company && database[data.target]->ownerType ==Company)
			{
				error = AddAssociateRelationship(data.owner, data.target);
			}
			else
			{
				error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Company ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
			}
			break;
		}
	}
	return error;
}

unique_ptr<Error> RelationshipDatabase::RemoveRelationship(RelationshipData data)
{
	unique_ptr<Error> error;
	switch (data.type)
	{
	case Parent:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
		{
			database[data.owner]->RemoveRelationship(data.target, Son);
			database[data.target]->RemoveRelationship(data.owner, Parent);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Son:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
		{
			database[data.target]->RemoveRelationship(data.owner, Son);
			database[data.owner]->RemoveRelationship(data.target, Parent);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Spouse:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
		{
			database[data.target]->RemoveRelationship(data.owner, Spouse);
			database[data.owner]->RemoveRelationship(data.target, Spouse);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Friend:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Simple)
		{
			database[data.owner]->RemoveRelationship(data.target, Friend); 
			((SimpleRelationships&)*database[data.target]).RemoveTargetedFriendship(data.owner);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Acquaintance:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType==Simple)
		{
			database[data.owner]->RemoveRelationship(data.target, Acquaintance);
			((SimpleRelationships&)*database[data.target]).RemoveTargetedAcquaintance(data.owner);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, "ID:" + to_string(data.owner)));
		}
		break;
	case Employer:
		if (database[data.owner]->ownerType == Company && database[data.target]->ownerType == Simple)
		{
			database[data.target]->RemoveRelationship(data.owner, Employer);
			database[data.owner]->RemoveRelationship(data.target, Employee);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Company ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Employee:
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Company)
		{
			database[data.target]->RemoveRelationship(data.owner, Employee);
			database[data.owner]->RemoveRelationship(data.target, Employer);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Member:		
		if (database[data.owner]->ownerType == Simple && database[data.target]->ownerType == Group)
		{
			database[data.target]->RemoveRelationship(data.owner, Member);
			database[data.owner]->RemoveRelationship(data.target, Membership);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Simple ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Membership:
		if (database[data.owner]->ownerType == Group && database[data.target]->ownerType == Simple)
		{
			database[data.target]->RemoveRelationship(data.owner, Membership);
			database[data.owner]->RemoveRelationship(data.target, Member);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Group ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	case Associate:
		if (database[data.owner]->ownerType == Company && database[data.target]->ownerType == Company)
		{
			database[data.target]->RemoveRelationship(data.owner, Associate);
			database[data.owner]->RemoveRelationship(data.target, Associate);
		}
		else
		{
			error = make_unique<Error>(Error(invalidUserType, database[data.owner]->ownerType == Company ? "ID:" + to_string(data.target) : "ID:" + to_string(data.owner)));
		}
		break;
	}
	return error;
}

unique_ptr<Error> RelationshipDatabase::AddUser(int id,UserType type)
{
	unique_ptr<Error> error;
	if (type == NotValid)
	{
		error = make_unique<Error>(notValidType);
	}
	if (!mValidId(id))
	{
		error = make_unique<Error>(invalidId, "ID:" + to_string(id));
	}
	else 
	{
		switch (type)
		{
		case Simple:
			database.insert(make_pair(id, make_unique<SimpleRelationships>(SimpleRelationships(id))));
			break;
		case Company:
			database.insert(make_pair(id, make_unique<CompanyRelationships>(CompanyRelationships(id))));
			break;
		case Group:
			database.insert(make_pair(id, make_unique<GroupRelationships>(GroupRelationships(id))));
			break;
		}
	}
	return error;
}

unique_ptr<Error> RelationshipDatabase::RemoveUser(int id)
{
	unique_ptr<Error> error;
	if (!mValidId(id) && database.find(id)==database.end())
	{
		error = make_unique<Error>(invalidId, "ID:" + to_string(id));
	}
	else 
	{
		unique_ptr<Error> error;
		Relationships& userRelationship = (SimpleRelationships&)*database[id];

		list<RelationshipData> relationshipsList;
		userRelationship.GetAllRelationships(relationshipsList);
		for (RelationshipData& Relationships : relationshipsList)
		{
			if (error == nullptr)
			{
				error = RemoveRelationship(Relationships);
			}
		}
		if (userRelationship.ownerType == Simple)
		{
			list<RelationshipData> targetedAcquaintancesData;
			for (int id:((SimpleRelationships&)userRelationship).GetTargetedAcquaintances())
			{
				targetedAcquaintancesData.push_front(RelationshipData(id, userRelationship.ownerId, Acquaintance));
			}
			for (RelationshipData& data : targetedAcquaintancesData)
			{
				error = RemoveRelationship(data);
			}

			list<RelationshipData> targetedFriendshipsData;
			for (int id : ((SimpleRelationships&)userRelationship).GetTargetedFriendships())
			{
				targetedFriendshipsData.push_front(RelationshipData(id, userRelationship.ownerId, Friend));
			}
			for (RelationshipData& data : targetedFriendshipsData)
			{
				error = RemoveRelationship(data);
			}
		}
		database.erase(id);
	}
	return error;
}

const Relationships & RelationshipDatabase::operator[](int id) const
{
	auto iter = database.find(id);
	if (iter != database.end())
	{
		return *(*iter).second;
	}
	return errorResult;
}

void RelationshipDatabase::VisitAllRelationships(function<void(const Relationships&)> visitingFunction) const
{
	for (auto& Relationships : database)
	{
		visitingFunction(*Relationships.second);
	}
}

unique_ptr<Error> RelationshipDatabase::CountRelatives(int id, int & counter) const
{
	return explorer.CountRelatives(id,counter);
}

unique_ptr<Error> RelationshipDatabase::GetFamilyTree(int id, list<UserGraphData>& tree) const
{
	return explorer.GetTree(id,tree);
}

void RelationshipDatabase::GetAllFamilyTrees(list<list<UserGraphData>>& trees) const
{
	set<int> visitedUsers; //it is used for finding the tree of an user already in a tree
	for (auto& relationships : database)
	{
		if (relationships.second->ownerType == Simple && visitedUsers.find(relationships.second->ownerId) == visitedUsers.end())
		{
			list<UserGraphData> newTree;
			if (explorer.GetTree(relationships.second->ownerId,newTree) == nullptr)
			{
				trees.push_front(newTree);
			}
			for (UserGraphData& data : newTree) // even if the tree is not valid its users are considered visited
			{
				visitedUsers.insert(data.GetOwner());
			}
		}
	}
}

unique_ptr<Error> RelationshipDatabase::AddParenthoodRelationship(int parentId, int sonId)
{
	unique_ptr<Error> error;

	SimpleRelationships& parent =(SimpleRelationships&) *database[parentId];
	SimpleRelationships& son =(SimpleRelationships&) *database[sonId];

	bool parentDuplicateCheck = Contains(parent.GetSons(), sonId);
	bool sonDuplicateCheck = Contains(son.GetParents(), parentId);

	if (!(parentDuplicateCheck && sonDuplicateCheck))
	{
		if (!son.AddRelationship(parentId, Parent) || !parent.AddRelationship(sonId, Son))
		{
			error = make_unique<Error>(Error(parentSonInverted, "ID genitore:" + to_string(parentId) + " ID figlio:" + to_string(sonId)));
		}
	}
	
	if (error == nullptr)
	{
		error = explorer.CheckValidFamilyTree(sonId);
		if (error != nullptr)
		{
			son.RemoveRelationship(parentId,Parent);
			parent.RemoveRelationship(sonId, Son);
		}
	}

	return error;
}

unique_ptr<Error> RelationshipDatabase::AddMarriageRelationship(int spouse1Id, int spouse2Id)
{
	unique_ptr<Error> error;

	SimpleRelationships& spouse1 = (SimpleRelationships&)*database[spouse1Id];
	SimpleRelationships& spouse2 = (SimpleRelationships&)*database[spouse2Id];

	if (!spouse1.AddRelationship(spouse2Id, Spouse) || !spouse2.AddRelationship(spouse1Id, Spouse))
	{
		int savedSpouse1;
		int savedSpouse2;

		spouse1.GetSpouse(savedSpouse1);
		spouse2.GetSpouse(savedSpouse2);

		if (!(savedSpouse1 == spouse2Id) || !(savedSpouse2 == spouse1Id))
		{
			error = make_unique<Error>(Error(invalidRelationship, "ID coniuge1:" + to_string(spouse1Id) + " ID coniuge2:" + to_string(spouse2Id)));
		}
	}

	return error;
}

unique_ptr<Error> RelationshipDatabase::AddFriendshipRelationship(int ownerId, int targetId)
{
	unique_ptr<Error> error;

	//controllo amiciazia con se stesso
	if (ownerId== targetId)
		error = make_unique<Error>(Error(invalidRelationship, "ID proprietario amicizia:" + to_string(ownerId) + " ID bersaglio di amicizia:" + to_string(targetId)));

	(*database[ownerId]).AddRelationship(targetId, Friend);
	((SimpleRelationships&)*database[targetId]).AddTargetedFriendship(ownerId);

	return error;
}

unique_ptr<Error> RelationshipDatabase::AddAcquaintanceshipRelationship(int ownerId, int targetId)
{
	unique_ptr<Error> error;

	if (ownerId == targetId)
		error = make_unique<Error>(Error(invalidRelationship, "ID proprietario conoscenza:" + to_string(ownerId) + " ID bersaglio di conoscenza:" + to_string(targetId)));

	(*database[ownerId]).AddRelationship(targetId, Acquaintance);
	((SimpleRelationships&)*database[targetId]).AddTargetedAcquaintance(ownerId);
	return error;
}

unique_ptr<Error> RelationshipDatabase::AddEmploymentRelationship(int ownerId, int targetId)
{
	database[ownerId]->AddRelationship(targetId,Employee);
	database[targetId]->AddRelationship(ownerId, Employer);
	return nullptr;
}

unique_ptr<Error> RelationshipDatabase::AddMembershipRelationship(int ownerId, int targetId)
{
	database[ownerId]->AddRelationship(targetId, Member);
	database[targetId]->AddRelationship(ownerId, Membership);
	return nullptr;
}

unique_ptr<Error> RelationshipDatabase::AddAssociateRelationship(int ownerId, int targetId)
{
	database[ownerId]->AddRelationship(targetId, Associate);
	database[targetId]->AddRelationship(ownerId, Associate);
	return nullptr;
}

RelationshipDatabase::ErrorRelationship::ErrorRelationship():Relationships(-1,NotValid)
{
}

bool RelationshipDatabase::ErrorRelationship::AddRelationship(int id, RelationshipType type)
{
	return false;
}

bool RelationshipDatabase::ErrorRelationship::RemoveRelationship(int id, RelationshipType type)
{
	return false;
}

void RelationshipDatabase::ErrorRelationship::GetAllRelationships(list<RelationshipData>& relationships) const
{
}
