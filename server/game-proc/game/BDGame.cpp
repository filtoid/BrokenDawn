#include "./BDGame.h"
#include "./BDUtils.h"
#include "./BDPlayer.h"

#include <string>
#include <curl/curl.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

/*Static Declarations*/

/*static*/ std::string BDGame::getItemFromDatabase(std::string db, std::string item)
{
	string retString;

	CURL *curl;
 	curl = curl_easy_init();
  	
	cout << "Finding item: " << item << endl;

	if(curl) {	
		std::ostringstream stringStream;
		stringStream <<  db << "/" << item;
  		std::string url = stringStream.str();
		
		cout << "...from url " << url;
	
		/* HTTP GET */ 
		curl_easy_setopt(curl, CURLOPT_URL, (char*)url.c_str());
	 	
  		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 	
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		cout << "About to easy perform" << endl;
		CURLcode res = curl_easy_perform(curl);  
		cout << "Returned from easy perform" <<endl;

		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		
		// Now copy the output to the return value
		std::string str(chunk.memory);
		retString = str;
  		cout << retString << " <= From func" << endl;
		curl_easy_cleanup(curl);
		cout << "Return from cleanup" << endl;

	}else
		cout<<"Failed to init curl" << endl;

	return retString;	
}

/*static*/ std::vector<std::string> BDGame::getVecOfGames()
{
	std::string gamesTable = BDGame::getItemFromDatabase("http://127.0.0.1:5984/test_db","bdgames");
	gamesTable.substr(0,gamesTable.find("}")); // Scrub any uknown chars	
	//cout << endl << "getVecOfGames: " << gamesTable;
		
	size_t start = gamesTable.find("\"games\"");
	start = gamesTable.find("[")+1; // +1 to avoid the bracket
	size_t end = gamesTable.find("]",start); // -1 avoid the bracket
	
	// start+1 to avoid the first square bracket
	std::string gameList = gamesTable.substr(start,(end-start));
	//cout << endl << "st:" << start << " en: " << end << "gamesList: " << gameList;

	std::vector<std::string> retVec;
 
	if(gameList.size()==0)
		return retVec;

	char chars[] = "[\"";
	for (unsigned int i = 0; i < strlen(chars); ++i)
		gameList.erase (std::remove(gameList.begin(), gameList.end(), chars[i]), gameList.end());	
	
	size_t next = gameList.find(",");
	while(gameList.find(",",next)!=-1){
		next = gameList.find(",",next);
		std::string nextStr = gameList.substr(0,next);
		gameList = gameList.substr(next+1,gameList.size()-next); 	
		retVec.push_back(nextStr);
	}
	
	// Last item should be only thing left
	retVec.push_back(gameList);
	
	// Debug to output - tests only - won't work in production
	//for(int i=0; i<retVec.size();i++){
	//	cout << i << ":" << retVec[i] << endl;		
	//}

 	return retVec;
}

/*static*/ int BDGame::getNumOfGames()
{ 
	return getVecOfGames().size();	
}

/*static*/ bool BDGame::isReadyToProcess(int id)
{
	return false;
}

/*static*/bool BDGame::process(int id)
{
	return false;
}

/*Non-Static Members*/

void BDGame::getGameFromJson( std::string json )
{		
	//size_t start = json.find("\"players\"");
	//start = json.find("[")+1; // +1 to avoid the bracket
	//size_t end = json.find("]",start); // -1 avoid the bracket
	std::string playersStr = BDUtils::getItemFromJson(json,"players");
	std::cout << "Returned from utils: " << playersStr;
	
	std::vector<std::string> players = BDUtils::getVecFromAry(playersStr);
	cout << "Size of players: " << players.size()<<endl;	
	for(int i=0;i<players.size();i++)
	{
		cout << "Next one: " << players[i] << endl;
		BDPlayer* player = new BDPlayer(players[i]);
		vecPlayers.push_back(player);
	}

	cout <<"Finished"<<endl;

	std::string versionstr = BDUtils::getItemFromJson(json,"version");
	float vernum( atof(versionstr.c_str() ) );	
	this->version = vernum;
}

BDGame::BDGame(std::string gameID)
{
	cout << "Makin new game: " << gameID  << endl;
	// Load game from the Id we've been given
	std::string gameJson = BDGame::getItemFromDatabase("http://127.0.0.1:5984/test_db",gameID);
	//cout << "Output" << gameJson;
	getGameFromJson(gameJson);
}

BDGame::~BDGame()
{
	/*for(int i=0;i<vecPlayers.size();i++)
	{
		delete vecPlayers[i];
		vecPlayers[i]=NULL;
	}*/
}

void BDGame::update()
{
	// Reload our settings and update anything we need to
}

int BDGame::getNumPlayers()
{
	return vecPlayers.size();
}

float BDGame::getVersion()
{
	return version;
}
	
int BDGame::getTurn()
{
	return turn;
}	
