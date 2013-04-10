#include "./TestUtils.h"

#include "BDUtils.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include <curl/curl.h>

using namespace std;

/*Static Setup/Tear down methods*/
// Vars to store the revision numbers of the table we have updated	
static std::string gamesRev="";
static std::string usersRev="";
static std::map<std::string,std::string> mapGameRev;
static std::map<std::string,std::string> mapPlayerRev;

/*static*/ void TestUtils::CreateDatabase()
{
	CURL *curl;
 
  	/* get a curl handle */ 
  	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db");
		 
		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			//fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		 
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;

}

/*static*/ void TestUtils::CreatePlayers(int num)
{

	// Create the players doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		ostringstream jsonStream;
		jsonStream << "{";
		
		if(gamesRev.compare(""))
			jsonStream << "\"_rev\":\""<< gamesRev << "\",";
 
		jsonStream << "\"players\": [";
		
		for(int i=0;i<num;i++){
			jsonStream << "\"Player" << i << "\"";
			if(i!=num-1)
				jsonStream<<",";						
		}

		jsonStream << "]}";
		string json = jsonStream.str();
		//cout << "json => " << json << endl;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdplayers");
		
		/*and the returned values*/
		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 	
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(chunk.memory);

		gamesRev = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

		curl_easy_cleanup(curl);
		
		for(int i=0;i<num;i++){
			ostringstream stream;
			stream << "Game" << i;
			TestUtils::CreateInstanceOfGame(stream.str(),false);
		}	

	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void TestUtils::CreateGames(int num, bool bCreateInstance/*=true*/)
{
	// Create the games doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		ostringstream jsonStream;
		jsonStream << "{";
		
		if(gamesRev.compare(""))
			jsonStream << "\"_rev\":\""<< gamesRev << "\",";
 
		jsonStream << "\"games\": [";
		
		for(int i=0;i<num;i++){
			jsonStream << "\"Game" << i << "\"";
			if(i!=num-1)
				jsonStream<<",";						
		}

		jsonStream << "]}";
		string json = jsonStream.str();
		//cout << "json => " << json << endl;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdgames");
		
		/*and the returned values*/
		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 	

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(chunk.memory);

		gamesRev = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

		curl_easy_cleanup(curl);
		
		for(int i=0;i<num && bCreateInstance;i++){
			ostringstream stream;
			stream << "Game" << i;
			TestUtils::CreateInstanceOfGame(stream.str(),false);
		}	

	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void TestUtils::DeleteDatabase()
{
	CURL *curl;
 
  	/* get a curl handle */ 
  	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP DELETE is a custom request */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); 
		/* use local couchdb*/
		curl_easy_setopt(curl, CURLOPT_URL,  "http://127.0.0.1:5984/test_db/");
		 
		CURLcode res = curl_easy_perform(curl);
		    
		/* Check for errors */ 
		if(res != CURLE_OK)
			cout << "curl error:" << curl_easy_strerror(res) << endl;
		 
		curl_easy_cleanup(curl);
	}
	
	// These revision numbers are no longer valid - because
	// we've just deleted the database.
	gamesRev = "";
	usersRev = "";
	mapGameRev.clear();
	
}

/*static*/ void TestUtils::CreateInstanceOfGame( std::string name, bool allPlayersReady /*=false*/ )
{
	// Create the default game details
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		ostringstream jsonStream;
		jsonStream << "{";
		
		if(mapGameRev["name"]!="")
			jsonStream << "\"_rev\":\""<< mapGameRev["name"] << "\",";
 
		jsonStream << "\"players\": [\"Player1\",\"Player2\"],\"turn\":26, ";
		
		if(allPlayersReady)
			jsonStream << "\"ready\": [\"yes\",\"yes\"], \"version\":0.1,\"Player1\":";
		else
			jsonStream << "\"ready\": [\"no\",\"no\"], \"version\":0.1,\"Player1\":";
		
		jsonStream << "\"player_game_1\",\"Player2\":\"player_game_2\"}";
						
		string json = jsonStream.str();
		//cout << "json => " << json << endl;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
 
		/* specify URL for creating the test_db */
		ostringstream urlstream;
		urlstream << "http://127.0.0.1:5984/test_db/" << name;
		curl_easy_setopt(curl, CURLOPT_URL, urlstream.str().c_str() );
		
		/*and the returned values*/
		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 			
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(chunk.memory);

		mapGameRev["name"] = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;
}


/*static*/ void CreateInstanceOfUser( std::string name )
{
	// Create the default player details
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		ostringstream jsonStream;
		jsonStream << "{";
		
		if(mapPlayerRev["name"]!="")
			jsonStream << "\"_rev\":\""<< mapPlayerRev["name"] << "\",";
 
		jsonStream << "\"name\": \"" << name << "\",\"email\":\"someone@somewhere.com\", ";
		jsonStream << "\"state\":\"WAITING\",";
		jsonStream << "\"games\": [ \"Game1\",\"Game2\"], \"version\":0.1,\"Game1\":";
		jsonStream << "\"player_game_1\",\"Game2\":\"player_game_2\"}";
						
		string json = jsonStream.str();
		cout << "json => " << json << endl;
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
 
		/* specify URL for creating the test_db */
		ostringstream urlstream;
		urlstream << "http://127.0.0.1:5984/test_db/" << name;
		curl_easy_setopt(curl, CURLOPT_URL, urlstream.str().c_str() );
		
		/*and the returned values*/
		struct MemoryStruct chunk;
 
  		chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */ 
  		chunk.size = 0;    /* no data at this point */ 			
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::WriteMemoryCallback );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(chunk.memory);

		mapPlayerRev["name"] = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;

}

