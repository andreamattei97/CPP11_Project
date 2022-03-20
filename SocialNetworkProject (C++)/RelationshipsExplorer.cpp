#include "RelationshipsExplorer.hpp"

#include <queue>

#include "SimpleRelationships.hpp"

RelationshipsExplorer::RelationshipsExplorer(function<const Relationships&(int)> getRelationships):getRelationships(getRelationships)
{
}

unique_ptr<Error> RelationshipsExplorer::CountRelatives(int id,int& counter) const
{
	unique_ptr<Error> error;
	const Relationships& relationships =(const Relationships& ) getRelationships(id);
	if (relationships.ownerType == NotValid)
	{
		error = make_unique<Error>(Error(userNotFound, "ID: " + to_string(id)));
	}
	else if (relationships.ownerType != Simple)
	{
		error = make_unique<Error>(Error(invalidUserType, "ID: " + to_string(id)));
	}

	if (error == nullptr)
	{
		counter = 0; 
		set<int> visitedUsers;
		CountElement(relationships.ownerId, counter, visitedUsers);
		counter--; //the user himself is not counted
	}

	return error;
}

unique_ptr<Error> RelationshipsExplorer::CheckValidFamilyTree(int id) const
{
	unique_ptr<Error> result;
	if (getRelationships(id).ownerType != NotValid) 
	{
		if (getRelationships(id).ownerType != Simple)
		{
			result = make_unique<Error>(Error(invalidUserType, "ID: " + to_string(id)));
		}
	}
	else
	{
		result = make_unique<Error>(Error(userNotFound, "ID: " + to_string(id)));
	}

	if (result == nullptr) 
	{
		map<int,UserGraphData> visitedUsers;
		set<int> recursionStack;

		if (SearchCycle(id,visitedUsers,recursionStack))
		{
			result = make_unique<Error>(Error(cycleFound, "ID: " + to_string(id)));
		}
	}
	return result;
}

unique_ptr<Error> RelationshipsExplorer::GetTree(int id, list<UserGraphData>& users) const
{
	unique_ptr<Error> error;
	if (getRelationships(id).ownerType != NotValid)
	{
		if (getRelationships(id).ownerType != Simple)
		{
			error = make_unique<Error>(Error(invalidUserType, "ID: " + to_string(id)));
		}
	}
	else
	{
		error = make_unique<Error>(Error(userNotFound, "ID: " + to_string(id)));
	}

	if (error == nullptr)
	{
		map<int,UserGraphData> visitedUsers;
		visitedUsers.insert(pair<int, UserGraphData>(id, UserGraphData(id)));
		queue<int> userQueue;
		bool valid = false; //a valid tree has at least 3 generations
		userQueue.push(id);
		while (userQueue.size() != 0)
		{
			int user = userQueue.front();
			userQueue.pop();
			const list<int>& sons = ((SimpleRelationships&)getRelationships(user)).GetSons();
			UserGraphData& userData = visitedUsers.at(user);
			for (int son : sons)
			{
				if (visitedUsers.find(son) == visitedUsers.end())
				{
					visitedUsers.insert(pair<int,UserGraphData>(son, UserGraphData(son)));
				}
				UserGraphData& sonData = visitedUsers.at(son);
				if (sonData.state == NotVisited)
				{
					userQueue.push(son);
					sonData.state = Enqueued;
				}
				userData.sons.push_front(son);
			}
			auto parents = ((SimpleRelationships&)getRelationships(user)).GetParents();
			for (int parent : parents)
			{
				if (visitedUsers.find(parent) == visitedUsers.end())
				{
					visitedUsers.insert(pair<int, UserGraphData>(parent, UserGraphData(parent)));
				}
				UserGraphData& parentData = visitedUsers.at(parent);
				if (parentData.state == NotVisited)
				{
					userQueue.push(parent);
					parentData.state = Enqueued;
				}
				if ( userData.parent1 == -1)
				{
					userData.parent1 = parent;
				}
				else 
				{
					userData.parent2 = parent;
				}
			}
			if (parents.size() != 0 && sons.size() != 0) //check if the tree is valid (3+ generations)
			{
				valid = true;
			}
			visitedUsers.at(user).state = Visited;
		}
		
		for (const pair<int,UserGraphData>& data : visitedUsers) //add the tree data in the list
		{
			users.push_front(data.second);
		}

		if (!valid)
		{
			error = make_unique<Error>(Error(notValidTree));
		}
	}
	return error;
}

bool RelationshipsExplorer::SearchCycle(int id, map<int,UserGraphData>& visitedUsers, set<int>& recursionSet) const
{
	if (visitedUsers.find(id) == visitedUsers.end())
	{
		visitedUsers.insert(pair<int,UserGraphData>(id, UserGraphData(id)));
	}
	recursionSet.insert(id);

	for (int son:((SimpleRelationships&)getRelationships(id)).GetSons())
	{
		if (visitedUsers.find(son) == visitedUsers.end() && SearchCycle(son, visitedUsers, recursionSet))
		{
			return true;
		}
		else if (recursionSet.find(son) != recursionSet.end())
		{
			return true;
		}
	}
	recursionSet.erase(id);
	return false;
}

void RelationshipsExplorer::CountElement(int id, int & counter, set<int>& visitedUsers) const
{
	visitedUsers.insert(id);
	counter++;
	const SimpleRelationships& relationships = (const SimpleRelationships&)getRelationships(id);
	for (int id : relationships.GetSons())
	{
		if (visitedUsers.find(id) == visitedUsers.cend())
		{
			CountElement(id, counter, visitedUsers);
		}
	}

	for (int id : relationships.GetParents())
	{
		if (visitedUsers.find(id) == visitedUsers.cend())
		{
			CountElement(id, counter, visitedUsers);
		}
	}

	int spouse = 0;
	if (relationships.GetSpouse(spouse) && (visitedUsers.find(spouse)==visitedUsers.cend()))
	{
		CountElement(spouse, counter, visitedUsers);
	}
}
