#include "GameProcTests.h"

#include "BDGame.h"

#include <iostream>
#include <string>
#include <sstream>

#include <cppunit/ui/text/TestRunner.h>

#include <curl/curl.h>

using namespace std;

GameProcTests::GameProcTests(){
	// Do the curl global setup
  	curl_global_init(CURL_GLOBAL_ALL);
}

GameProcTests::~GameProcTests(){
	// Do the curl global cleanup
  	curl_global_cleanup();
}

/*static*/ void GameProcTests::setup( CppUnit::TestSuite* suite)
{
	// Add all of our tests
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testGetNumGames",
			&GameProcTests::testGetNumGames ) );
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testGetGame",
			&GameProcTests::testGetGame ) );
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testIsGameReady",
			&GameProcTests::testIsGameReady ) );
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testProcessGame",
			&GameProcTests::testProcessGame ) );
}

/*Static Setup/Tear down methods*/

/*static*/ void GameProcTests::CreateDatabase()
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
/*static*/ void GameProcTests::CreateUsers()
{
	// Create the users doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"users\": [\"1stUser\",\"2ndUser\"]}");
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdusers");
		 
		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		 
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void GameProcTests::CreateGames()
{
	// Create the games doc in the test db
	CURL *curl;
 	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP PUT */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"games\": [\"Game1\",\"Game2\"]}");
 
		/* specify URL for creating the test_db */ 
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5984/test_db/bdgames");
		 
		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK)
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
		 
		curl_easy_cleanup(curl);
	}else
		cout<<"Failed to init curl" << endl;
}

/*static*/ void GameProcTests::DeleteDatabase()
{
	CURL *curl;
 
  	/* get a curl handle */ 
  	curl = curl_easy_init();
  	
	if(curl) {	
		/* HTTP DELETE is a custom request */ 
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); 
		/* use local couchdb*/
		curl_easy_setopt(curl, CURLOPT_URL,  "http://127.0.0.1:5984/test_db");
		 
		CURLcode res = curl_easy_perform(curl);
		    
		/* Check for errors */ 
		if(res != CURLE_OK)
			cout << "curl error:" << curl_easy_strerror(res) << endl;
		//fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
		 
		curl_easy_cleanup(curl);
	}
}

/*Test methods*/

void GameProcTests::testGetNumGames()
{
	cout << "Test get number of games.";
	// Setup
	
	cout << "init.";
	// Do setup - set up couchDB etc
	
	GameProcTests::CreateDatabase();
	GameProcTests::CreateUsers();
	GameProcTests::CreateGames();

	cout << "cleanup.";
	// Reset everything just the way they were
	GameProcTests::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testGetGame()
{

}

void GameProcTests::testIsGameReady()
{

}

void GameProcTests::testProcessGame()
{

}
