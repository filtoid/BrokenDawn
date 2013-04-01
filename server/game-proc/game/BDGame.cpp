#include "./BDGame.h"
#include "./BDUtils.h"
#include <string>
#include <curl/curl.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

BDGame::BDGame()
{

}

/*static*/ std::string BDGame::getItemFromDatabase(std::string db, std::string item)
{
	string retString;

	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		std::ostringstream stringStream;
		stringStream <<  db << "/" << item;
  		std::string url = stringStream.str();
		
		/* HTTP GET */ 
		curl_easy_setopt(curl, CURLOPT_URL, (char*)url.c_str());
	 	struct bdstring s;  // Make the structure for the return value
	 	s.len = 0;
  		s.ptr = (char *)malloc(s.len+1);
  		if (s.ptr == NULL) {
			cout<< "malloc() failed";    			
			fprintf(stderr, "malloc() failed\n");
			exit(EXIT_FAILURE);
  		}
  		s.ptr[0] = '\0';

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::curl_write );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		CURLcode res = curl_easy_perform(curl);  
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		
		// Now copy the output to the return value
		std::string str(s.ptr);
		retString = str;
  
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;

	return retString;	
}

std::vector<std::string> BDGame::getVecOfGames()
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

int BDGame::getNumOfGames()
{ 
	return getVecOfGames().size();	
}

bool BDGame::isReadyToProcess(int id)
{
	return false;
}

bool BDGame::process(int id)
{
	return false;
}
