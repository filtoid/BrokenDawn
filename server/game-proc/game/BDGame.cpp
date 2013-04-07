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

/*static*/ std::vector<std::string> BDGame::getVecOfGames()
{
	// Get the whole list
	std::string gamesTable = BDUtils::getItemFromDatabase("http://127.0.0.1:5984/test_db","bdgames");
	gamesTable = gamesTable.substr(0,gamesTable.find("}")); // Scrub any uknown chars	
	
	// Get just the array of data we want	
	std::string gameList = BDUtils::getItemFromJson(gamesTable,"games");	
	
 	return BDUtils::getVecFromAry(gameList);
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
	// Extract all game info from json
	std::string playersStr = BDUtils::getItemFromJson(json,"players");	
	std::vector<std::string> players = BDUtils::getVecFromAry(playersStr);

	// Make up player vector
	for(int i=0;i<players.size();i++)
	{
		BDPlayer player(players[i]);
		vecPlayers.push_back(player);
	}
	
	// Get player ready value
	std::string readyStr = BDUtils::getItemFromJson(json,"ready");
	std::vector<std::string> readyVec = BDUtils::getVecFromAry(readyStr);
	for(int i=0;i<readyVec.size();i++){
		if(readyVec[i].compare("yes"))
			vecReady.push_back(true);
		else
			vecReady.push_back(false);	
	}

	// Version number
	std::string versionstr = BDUtils::getItemFromJson(json,"version");
	float vernum( atof(versionstr.c_str() ) );	
	this->version = vernum;

	// Turn number
	std::string turnStr = BDUtils::getItemFromJson(json,"turn");
	this->turn = atoi( turnStr.c_str() );
}

BDGame::BDGame(std::string gameID)
{
	// Load game from the Id we've been given
	std::string gameJson = BDUtils::getItemFromDatabase("http://127.0.0.1:5984/test_db",gameID);
	getGameFromJson(gameJson);
}

BDGame::~BDGame()
{
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
