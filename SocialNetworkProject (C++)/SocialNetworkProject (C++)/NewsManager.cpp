#include "NewsManager.hpp"

#include <ctime>
#include <regex>



NewsManager::NewsManager(ApplicationInterface& applicationInterface, NewsDatabase& database,const UserDatabase& userDatabase):
	database(database),applicationInterface(applicationInterface),userDatabase(userDatabase)
{
}

void NewsManager::ManageNews()
{
	int id;
	bool aborted;
	aborted = !applicationInterface.intInput(id, "Inserisci l'id con cui vuoi entrare (-1 per rimuovere notizie come amministratore)", true, [this](int id) {return database.idValidCondition(id) || id == -1; });

	if (!aborted)
	{
		if (id != -1) 
		{
			int option;
			bool finished = false;
			applicationInterface.PrintMessage("Benvenuto/a " + userDatabase[id].GetUsername());
			while (!finished)
			{
				aborted = !applicationInterface.intInput(option, "Comandi: 0=bacheca notizie, 1=inserisci notizia, 2=elimina notizia, 3=esci"
					, false, [](int option) {return option < 4 && option >= 0; });
				if (!aborted)
				{
					switch (option)
					{
					case 0:
						Showcase(id);
						break;
					case 1:
						AddNews(id);
						break;
					case 2:
						RemoveNews(id);
						break;
					case 3:
						finished = true;
						break;
					}
				}
			}
		}
		else
		{
			AdminMode();
		}
	}
}

void NewsManager::Showcase(int id)
{
	ShowcaseHandler handler = database.GetHomepageHandler();
	bool finished = false;
	while (!finished)
	{
		applicationInterface.PrintMessage("Bacheca generale:");
		auto elements = handler.GetPageElements();
		int elementsSize = elements.size();
		if (elementsSize != 0)
		{
			for (int i = 0; i < elementsSize; i++)
			{
				applicationInterface.PrintNews(elements[i], userDatabase[elements[i].get().ownerId].GetUsername(), 1, i);
			}
		}
		else
		{
			applicationInterface.PrintMessage("Nessuna notizia trovata");
		}

		int option;
		
		//the command help string is dinamycally build	
		string inputMessage = "Comandi: ";	
		if (elementsSize != 0)
		{
			if (elementsSize > 1)
				inputMessage += "0-" + to_string(elementsSize - 1) + "=seleziona la notizia corrispondente";
			else
				inputMessage += "0" + to_string(elementsSize - 1) + "=seleziona la notizia corrispondente";
		}
		inputMessage+= " 10=scorri pagina 11=esci";

		if (applicationInterface.intInput(option, inputMessage, true, [elementsSize](int i) {return (i>=0 && i<elementsSize) || (i==10) || (i==11); }))
		{
			if (option >= 0 && option < elementsSize)
			{
				ManageSingleNews(handler, option, id);
			}
			else if (option == 10)
			{
				handler.LoadNextPage();
			}
			else if(option == 11)
			{
				finished = true;
			}
		}
		else 
		{
			finished = true;
		}
	}
}

void NewsManager::ManageSingleNews(ShowcaseHandler& handler, int elementNumber, int userId)
{
	const News& news = handler.GetPageElements()[elementNumber];
	const User& user = userDatabase[news.ownerId];
	bool alreadyLiked = false, alreadyDisliked = false;

	applicationInterface.PrintNews(news, user.GetUsername());

	auto likes = news.likesList();
	applicationInterface.PrintMessage("Mi piace:");
	for (int like : likes)
	{
		applicationInterface.PrintMessage(userDatabase[like].GetUsername(), 1);
		if (like == userId)
		{
			alreadyLiked = true;
		}
	}

	auto dislikes = news.dislikesList();
	applicationInterface.PrintMessage("Non mi piace:");
	for (int dislike : dislikes)
	{
		applicationInterface.PrintMessage(userDatabase[dislike].GetUsername(), 1);
		if (dislike == userId)
		{
			alreadyDisliked = true;
		}
	}

	int option;
	if (!alreadyLiked && !alreadyDisliked && news.ownerId != userId)
	{
		if (applicationInterface.intInput(option, "Comandi: 0=mi piace, 1=non mi piace, 2=esci", false, [](int i) {return i == 0 || i == 1 || i == 2; }))
		{
			if (option == 0)
			{
				handler.InsertLike(elementNumber, userId);
				applicationInterface.PrintMessage("Mi piace messo con successo");
			}

			if (option == 1)
			{
				handler.InsertDislike(elementNumber, userId);
				applicationInterface.PrintMessage("Non mi piace messo con successo");
			}
		}
	}
	else
	{
		if (alreadyLiked)
		{
			bool result;
			applicationInterface.PrintMessage("Trovato mi piace");
			if (applicationInterface.QuestionInput(result, "Vuoi rimuovere il mi piace? (preme Y per rimuovere)") && result)
			{
				handler.RemoveLike(elementNumber, userId);			
				applicationInterface.PrintMessage("Mi piace rimosso con successo");
			}
		}
		else if (alreadyDisliked)
		{
			bool result;
			applicationInterface.PrintMessage("Trovato non mi piace");
			if (applicationInterface.QuestionInput(result, "Vuoi rimuovere il non mi piace? (preme Y per rimuovere)") && result)
			{
				handler.RemoveDislike(elementNumber, userId);
				applicationInterface.PrintMessage("Non mi piace rimosso con successo");
			}
		}
		else
		{
			applicationInterface.PrintMessage("Non puoi mettere mi piace o non mi piace ad una tua notizia");
		}
	}
}

void NewsManager::AddNews(int id)
{
	bool aborted;
	string message;

	aborted = !applicationInterface.lineInput(message, "Inserisci il testo", true,
		[this](string s) {return regex_match(s, messageRegex); });
	if (!aborted)
	{
		database.InsertNews(id, message);
		applicationInterface.PrintMessage("Notizia inserita con succeso");
	}
}

void NewsManager::RemoveNews(int id)
{
	unique_ptr<UserNewsHandler> handler = database.GetUserNewsHandler(id);
	bool finished = false;
	while (!finished)
	{
		applicationInterface.PrintMessage("Bacheca notizie utente:");
		auto elements = handler->GetPageElements();
		int elementsSize = elements.size();
		if (elementsSize != 0)
		{
			for (int i = 0; i < elementsSize; i++)
			{
				applicationInterface.PrintNews(elements[i], userDatabase[elements[i].get().ownerId].GetUsername(), 1, i);
			}
		}
		else
		{
			applicationInterface.PrintMessage("Nessuna notizia trovata");
		}

		int option;
		//the command help string is dinamycally build
		string inputMessage = "Comandi: ";
		if (elementsSize != 0)
		{
			if(elementsSize>1)
				inputMessage += "0-" + to_string(elementsSize - 1) + "=seleziona la notizia corrispondente";
			else
				inputMessage += "0" + to_string(elementsSize - 1) + "=seleziona la notizia corrispondente";
		}
		inputMessage += " 10=scorri pagina 11=esci"; 
		if (applicationInterface.intInput(option, inputMessage, true, [elementsSize](int i) {return (i >= 0 && i < elementsSize) || (i == 10) || (i == 11); }))
		{
			if (option >= 0 && option < elementsSize)
			{
				handler->RemoveNews(option);
			}
			else if (option == 10)
			{
				handler->LoadNextPage();
			}
			else if (option == 11)
			{
				finished = true;
			}
		}
		else
		{
			finished = true;
		}
	}
}

void NewsManager::AdminMode()
{
	ShowcaseHandler handler = database.GetHomepageHandler();
	bool finished = false;
	while (!finished)
	{
		applicationInterface.PrintMessage("Bacheca amministratore:");
		auto elements = handler.GetPageElements();
		int elementsSize = elements.size();
		if (elementsSize != 0)
		{
			for (int i = 0; i < elementsSize; i++)
			{
				applicationInterface.PrintNews(elements[i], userDatabase[elements[i].get().ownerId].GetUsername(), 1, i);
			}
		}
		else
		{
			applicationInterface.PrintMessage("Nessuna notizia trovata");
		}

		int option;
		string inputMessage = "Comandi: ";
		if (elementsSize != 0)
			inputMessage += "0-" + to_string(elementsSize - 1) + "=seleziona la notizia corrispondente";
		inputMessage += "10=scorri pagina 11=esci"; 
		if (applicationInterface.intInput(option, inputMessage, true, [elementsSize](int i) {return (i >= 0 && i < elementsSize) || (i == 10) || (i == 11); }))
		{
			if (option >= 0 && option < elementsSize)
			{
				handler.RemoveNews(option);
			}
			else if (option == 10)
			{
				handler.LoadNextPage();
			}
			else if (option == 11)
			{
				finished = true;
			}
		}
		else
		{
			finished = true;
		}
	}
}

void NewsManager::SaveDatabase()
{
	unique_ptr<Error> error = database.SaveDatabase();
	if (error != nullptr)
	{
		applicationInterface.PrintError(*error);
	}
	else
	{
		applicationInterface.PrintMessage("Database notizie salvato");
	}
}

