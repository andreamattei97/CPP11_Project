#include "SearchesManager.hpp"

SearchesManager::SearchesManager(const UserDatabase & userDatabase, const RelationshipDatabase & relationshipDatabase, const NewsDatabase & newsDatabase, const ApplicationInterface & applicationInterface):
	userDatabase(userDatabase),relationshipDatabase(relationshipDatabase),newsDatabase(newsDatabase),applicationInterface(applicationInterface)
{
}

void SearchesManager::ManageSearches()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Scegli il tipo di ricerca: 0=alberi genealogici, 1=lupi solitari, 2=ricerca simpatia, 3=esci", true, [](int option) {return option >= 0 && option < 4; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				FamiltyTree();
				break;
			case 1:
				LoneWolves();
				break;
			case 2:
				SympathySearch();
				break;
			case 3:
				finished = true;
				break;
			}
		}
	}
}

void SearchesManager::FamiltyTree()
{
	int option;
	bool finished = false;
	while (!finished)
	{
		bool aborted = !applicationInterface.intInput(option, "Scegli il tipo di ricerca: 0=albero utente, 1=tutti gli alberi, 2=esci", false, [](int option) {return option >= 0 && option < 3; });

		if (!aborted)
		{
			switch (option)
			{
			case 0:
				SingleTree();
				break;
			case 1:
				AllTrees();
				break;
			case 2:
				finished = true;
				break;
			}
		}
	}
}

void SearchesManager::LoneWolves()
{
	bool aborted = false;
	//friends/acquaintances threshold
	int minRelationships=0;
	//relatives threshold
	int minRelatives=0;
	//employments threshold
	int minEmployments = 0;
	//memberships threshold
	int minMemberships = 0;
	//likes/dislike/news published threshold
	int minNewsInteractions = 0;

	aborted = !applicationInterface.intInput(minRelationships,
		"Inserire il numero minimo di amicizie/conoscenze per non essere considerati lupi solitari",
		true, [](int id) {return id >= 0; });
	if (!aborted)
	{
		aborted = !applicationInterface.intInput(minRelatives,
			"Inserire il numero minimo di parenti per non essere considerati lupi solitari",
			true, [](int id) {return id >= 0; });
	}
	
	if (!aborted)
	{
		aborted = !applicationInterface.intInput(minEmployments,
			"Inserire il numero minimo di impieghi per non essere considerati lupi solitari",
			true, [](int id) {return id >= 0; });
	}

	if (!aborted)
	{
		aborted = !applicationInterface.intInput(minMemberships,
			"Inserire il numero minimo di iscrizioni a gruppi per non essere considerati lupi solitari",
			true, [](int id) {return id >= 0; });
	}

	if (!aborted)
	{
		aborted = !applicationInterface.intInput(minNewsInteractions,
			"Inserire il numero minimo di interazioni con le notizie per non essere considerati lupi solitari",
			true, [](int id) {return id >= 0; });
	}

	if (!aborted)
	{
		list<int> loneWolves;
		//contains the number of interactions in news of every simple user (the list of users is initialized using the Relationships database)
		map<int, int> newsInteractions;
		relationshipDatabase.VisitAllRelationships([this,&newsInteractions,&loneWolves,minRelationships,minMemberships,minEmployments,minRelatives]
		(const Relationships& relationships) {
			if (relationships.ownerType == Simple)
			{
				const SimpleRelationships& simpleRel= (const SimpleRelationships&)relationships;
				int relatives;
				relationshipDatabase.CountRelatives(simpleRel.ownerId, relatives);
				int friendsAcquaintaces = simpleRel.GetFriends().size() + simpleRel.GetAcquaintances().size();
				int memberships = simpleRel.GetMemberships().size();
				int employments = simpleRel.GetEmployers().size();
				if ( relatives < minRelatives && find(loneWolves.begin(), loneWolves.end(), simpleRel.ownerId) == loneWolves.end())
				{
					loneWolves.push_back(simpleRel.ownerId);
				}
				else if (friendsAcquaintaces < minRelationships && find(loneWolves.begin(), loneWolves.end(), simpleRel.ownerId) == loneWolves.end())
				{
					loneWolves.push_back(simpleRel.ownerId);
				}
				else if (memberships < minMemberships && find(loneWolves.begin(), loneWolves.end(), simpleRel.ownerId) == loneWolves.end())
				{
					loneWolves.push_back(simpleRel.ownerId);
				}
				else if (employments < minEmployments && find(loneWolves.begin(), loneWolves.end(), simpleRel.ownerId) == loneWolves.end())
				{
					loneWolves.push_back(simpleRel.ownerId);
				}
				//adds the user in the newsInteraction count
				newsInteractions[simpleRel.ownerId] = 0;
			}
		});
		
		newsDatabase.VisitAllNews([&loneWolves, &newsInteractions](const News& news){
			if (newsInteractions.find(news.ownerId) != newsInteractions.end())
				newsInteractions[news.ownerId]++;
			for (int like : news.likesList())
			{
				if (newsInteractions.find(like) != newsInteractions.end())
					newsInteractions[like]++;
			}
			for (int dislike : news.dislikesList())
			{
				if (newsInteractions.find(dislike) != newsInteractions.end())
					newsInteractions[dislike]++;
			}
		});

		for (const auto& userInteractions : newsInteractions)
		{
			if (userInteractions.second < minNewsInteractions && find(loneWolves.begin(), loneWolves.end(), userInteractions.first) == loneWolves.end())
				loneWolves.push_back(userInteractions.first);
		}

		if (loneWolves.size() != 0)
			applicationInterface.PrintMessage("Lista lupi solitari:");
		for (int loneWolf : loneWolves)
		{
			applicationInterface.PrintMessage(userDatabase[loneWolf].GetUsername(), 1);
		}
	}
}

void SearchesManager::SympathySearch()
{
	bool aborted = false;
	float ratio;
	bool countAssociates = false;
	aborted = !applicationInterface.floatInput(ratio, "Immettere il rapporto minimo apprazzamenti e indignazioni", true, [](float f) {return f >= 0; });
	if (!aborted)
	{
		aborted = !applicationInterface.QuestionInput(countAssociates, "Considerare anche le consociate nel conteggio (premere Y per confermare)", true);
	}

	if (!aborted)
	{
		map<int, NewsNavigationData> users;
		relationshipDatabase.VisitAllRelationships([&users,countAssociates](const Relationships& relationships) {
			if (relationships.ownerType == Company)
			{
				NewsNavigationData data;
				data.id = relationships.ownerId;
				if (countAssociates)
				{
					const list<int>& associates = ((const CompanyRelationships&)relationships).GetAssociates();
					data.associates=list<int>( associates.begin(), associates.end());
				}
				users[relationships.ownerId] = data;
			}
		});
		for (auto& user : users)
		{
			newsDatabase.VisitAllNews([&user](const News& news) {
				if (news.ownerId == user.first || find(user.second.associates.begin(), user.second.associates.end(), news.ownerId) != user.second.associates.end())
				{
					user.second.likes += news.likesList().size();
					user.second.dislikes += news.dislikesList().size();
				}
			});
		}
		list<reference_wrapper<const NewsNavigationData>> validUsers;
		for (const auto& user : users)
		{
			if (user.second.dislikes != 0)
			{
				if ((((float)user.second.likes) / user.second.dislikes) >= ratio)
				{
					validUsers.push_front(user.second);
				}
			}
			else if (user.second.likes != 0)
			{
				validUsers.push_front(user.second);
			}
		}

		if (validUsers.size() != 0)
		{
			//the list is sorted in a decreasing order
			validUsers.sort([](const NewsNavigationData& data1,const NewsNavigationData& data2) {
				float ratio1= data1.dislikes != 0 ? ((float)data1.likes) / data1.dislikes : numeric_limits<float>::max();
				float ratio2= data2.dislikes != 0 ? ((float)data2.likes) / data2.dislikes : numeric_limits<float>::max();
				if (ratio2 > ratio1)
					return false;
				if (ratio1 == ratio2 && data1.likes < data2.likes) //in case of equal ratio the one with more likes wins
					return false;
				if (ratio1 == ratio2 && data1.likes == data2.likes && data1.dislikes > data2.dislikes) //in case of equal likes the one with the least dislikes wins
					return false;
				if(ratio1 == ratio2 && data1.likes == data2.likes && data1.dislikes == data2.dislikes) //in a draw the one with a lower id wins
					return data1.id > data2.id;
				return true;
			});
			applicationInterface.PrintMessage("Classifica simpatia aziende");
			for (const auto& user : validUsers)
			{
				float userRatio = user.get().dislikes != 0 ? ((float)user.get().likes) / user.get().dislikes : numeric_limits<float>::max();
				if (userRatio != numeric_limits<float>::max())
					applicationInterface.PrintStatistic(userDatabase[user.get().id].GetName(), userRatio, 2);
				else
					applicationInterface.PrintMessage(userDatabase[user.get().id].GetName() + ": nessuna indignazione e "+ to_string(user.get().likes)+" apprezzamenti");
			}
		}
		else
		{
			applicationInterface.PrintMessage("nessun utente valido trovato");
		}
	}
}

void SearchesManager::SingleTree()
{
	bool aborted = false;
	int id;

	aborted = !applicationInterface.intInput(id, "Inserisci l'id dell'utente di cui vuoi costruire l'albero", true, [this](int id) {return userDatabase.ContainsId(id) && userDatabase[id].userType==Simple; });
	if (!aborted)
	{
		list<UserGraphData> treeData;
		unique_ptr<Error> error = relationshipDatabase.GetFamilyTree(id, treeData);
		if (error == nullptr)
		{
			list<string> treeStrings;
			applicationInterface.PrintMessage("Albero di "+userDatabase[id].GetUsername()+" trovato:");
			for (auto& data: treeData)
			{
				treeStrings.push_front(userDataString(data));
				applicationInterface.PrintMessage(treeStrings.front(),1);
			}
			bool save = false;
			applicationInterface.QuestionInput(save, "Vuoi salvare l'albero su un file? (inserire Y per confermare, un qualunque altro tasto per annullare)");
			if (save)
			{
				FileManager file("Albero di " + userDatabase[id].GetUsername() + ".txt");
				error=file.WriteListFile(treeStrings.begin(), treeStrings.end());
				if (error == nullptr)
				{
					applicationInterface.PrintMessage("Albero salvato con successo");
				}
				else
				{
					applicationInterface.PrintError(*error);
				}
			}
		}
		else
		{
			applicationInterface.PrintError(*error);
		}
	}
}

void SearchesManager::AllTrees()
{
	list<list<UserGraphData>> trees;
	relationshipDatabase.GetAllFamilyTrees(trees);
	if (trees.size() != 0)
	{
		bool save = false;
		applicationInterface.QuestionInput(save, "Vuoi salvare gli alberi su file? (inserire Y per confermare, un qualunque altro tasto per annullare)");
		for (auto& tree : trees)
		{
			list<string> treeStrings;
			string username = userDatabase[tree.front().GetOwner()].GetUsername();
			applicationInterface.PrintMessage("Albero di " + username);
			for (UserGraphData& data : tree)
			{
				treeStrings.push_front(userDataString(data));
				applicationInterface.PrintMessage(treeStrings.front(), 1);
			}
			if (save)
			{
				FileManager file("Albero di " + username + ".txt");
				unique_ptr<Error> error = file.WriteListFile(treeStrings.begin(), treeStrings.end());
				if (error == nullptr)
				{
					applicationInterface.PrintMessage("Albero salvato con successo");
				}
				else
				{
					applicationInterface.PrintError(*error);
				}
			}
		}
	}
	else
	{
		applicationInterface.PrintMessage("Nessun albero valido trovato");
	}
}

string SearchesManager::userDataString(const UserGraphData & user)
{
	string result=userDatabase[user.GetOwner()].GetUsername();
	if (user.parent1 != -1)
		result += ", figlio di " + userDatabase[user.parent1].GetUsername();
	if (user.parent2 != -1)
		result += " e di " + userDatabase[user.parent2].GetUsername();
	if (user.sons.size() > 0)
	{
		result += ", genitore di: ";
		auto iter = user.sons.begin();
		result += userDatabase[*iter].GetUsername();
		iter++;
		for (; iter != user.sons.end(); iter++)
		{
			result += ", " + userDatabase[*iter].GetUsername();
		}
	}
	return result;
}
