#include "RelationshipManager.hpp"

RelationshipManager::RelationshipManager(UserDatabase& userDatabase, RelationshipDatabase& relationshipDatabase, ApplicationInterface& applicationInterface):
	userDatabase(userDatabase),relationshipDatabase(relationshipDatabase),applicationInterface(applicationInterface)
{
}

void RelationshipManager::ManageRelationships()
{
	int id;
	bool aborted;
	aborted = !applicationInterface.intInput(id, "Inserisci l'id dell'utente di cui vuoi gestire le relazioni", true, bind(&UserDatabase::ContainsId, &userDatabase, placeholders::_1));
	
	if (!aborted)
	{
		int option;
		bool finished=false;
		applicationInterface.PrintMessage("Gestione relazioni di " + userDatabase[id].GetUsername());
		while (!finished)
		{
			aborted = !applicationInterface.intInput(option, "Comandi: 0=visualizza relazioni, 1=aggiungi relazione, 2=rimuovi relazione, 3=esci da gestione relazioni"
				, false, [](int option) {return option < 4 && option >= 0; });
			if (!aborted)
			{
				switch (option)
				{
				case 0:
					VisualizeRelationships(id);
					break;
				case 1:
					AddRelationship(id);
					break;
				case 2:
					RemoveRelationship(id);
					break;
				case 3:
					finished = true;
					break;
				}
			}
		}
	}
}

void RelationshipManager::VisualizeRelationships(int id)
{
	list<RelationshipData> relationships;
	relationshipDatabase[id].GetAllRelationships(relationships);
	for (const RelationshipData& data : relationships)
	{
		userDatabase[data.target];
		string relationshipMessage = relationshipNames[data.type] + " di " + userDatabase[data.target].GetName() + " (id:" + to_string(data.target)+")";
		applicationInterface.PrintMessage(relationshipMessage);
	}
}

void RelationshipManager::AddRelationship(int id)
{
	int targetId;
	bool aborted;
	aborted = !applicationInterface.intInput(targetId, "Inserisci l'id dell'utente destinatario della relazione",true,bind(&UserDatabase::ContainsId,&userDatabase,placeholders::_1));

	if (!aborted)
	{
		int option;
		aborted = !applicationInterface.intInput(option,
			"Scegli il tipo di relazione da inserire\n0=genitore, 1=figlio, 2=coniuge, 3=amico, 4=conoscente, 5=datore di lavoro, 6=impiegato, 7=membro, 8=iscrizione, 9=associata",
			true, [](int i) {return i <= 9 && i >= 0; });
		if (!aborted)
		{
			RelationshipData Relationships(id, targetId, (RelationshipType)option);
			unique_ptr<Error> error = relationshipDatabase.AddRelationship(Relationships);
			if (error != nullptr)
			{
				applicationInterface.PrintError(*error);
			}
			else
			{
				applicationInterface.PrintMessage("Inserimento relazione avvenuto con successo");
			}
		}
	}
}

void RelationshipManager::RemoveRelationship(int id)
{
	applicationInterface.PrintMessage("Relazioni dell'utente:");
	VisualizeRelationships(id);

	list<RelationshipData> data;
	relationshipDatabase[id].GetAllRelationships(data);

	int targetId;
	bool aborted;

	condition<int> targetIdCondition = [&data](int i) {
		auto iter=find_if(data.cbegin(), data.cend(), [i](RelationshipData Relationships) {
			return Relationships.target == i;
		});
		return iter != data.cend();
	};
	
	aborted = !applicationInterface.intInput(targetId, "Inserisci l'id dell'utente destinatario della relazione che vuoi eliminare", true, targetIdCondition);

	if (!aborted)
	{
		condition<int> relationshipTypeCondition = [&data, targetId](int type) {
			if (type < 0 || type >9)
				return false;
			auto iter = find_if(data.cbegin(), data.cend(), [targetId,type](RelationshipData Relationships) {
				return Relationships.target == targetId && Relationships.type == (RelationshipType)type;
			});
			return iter != data.cend();
		};

		int type;

		aborted = !applicationInterface.intInput(type, "Inserisci il tipo della relazione da eliminare\n0=genitore, 1=figlio, 2=coniuge, 3=amico, 4=conoscente, 5=datore di lavoro, 6=impiegato, 7=membro, 8=iscrizione, 9=associata", true, relationshipTypeCondition);

		if (!aborted)
		{
			RelationshipData Relationships(id, targetId,(RelationshipType) type);
			unique_ptr<Error> error = relationshipDatabase.RemoveRelationship(Relationships);
			if (error == nullptr)
			{
				applicationInterface.PrintMessage("Relazione eliminata con successo");
			}
			else
			{
				applicationInterface.PrintError(*error);
			}
		}
	}
}

void RelationshipManager::SaveDatabase()
{
	unique_ptr<Error> error = relationshipDatabase.SaveDatabase();
	if (error != nullptr)
	{
		applicationInterface.PrintError(*error);
	}
	else
	{
		applicationInterface.PrintMessage("Database relazioni salvato");
	}
}
