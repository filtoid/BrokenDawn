#include "./BDGame.h"
#include "./BDUtils.h"
#include <string>
#include <curl/curl.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>

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

int BDGame::getNumOfGames()
{ 
	//int[] retAry = new int[1];
	//retAry[0] = "1";
	std::string gamesTable = BDGame::getItemFromDatabase("http://127.0.0.1:5984/test_db","bdgames");
	//cout << "output: " << gamesTable;
	size_t start = gamesTable.find("\"games\"");
	start = gamesTable.find("[")+1; // +1 to avoid the bracket
	size_t end = gamesTable.find("]",start)-1; // -1 avoid the bracket
	
	// start+1 to avoid the first square bracket
	std::string gameList = gamesTable.substr(start,(end-start));
	char chars[] = "[\"";
	for (unsigned int i = 0; i < strlen(chars); ++i)
		gameList.erase (std::remove(gameList.begin(), gameList.end(), chars[i]), gameList.end());	
	
  	int count = 0;
  	for (int i = 0; i < gameList.size(); i++)
    	{
		// Count the number of commas (the seperation char)
		if (gameList[i] == ',') 
			count++;
	}
	count +=1 ; // Because the last item doesn't have a comma

 	return count;
}

bool BDGame::isReadyToProcess(int id)
{
	return false;
}

bool BDGame::process(int id)
{
	return false;
}
