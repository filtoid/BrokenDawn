#include "./GameProcTests.h"

#include "BDGame.h"
#include "BDUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <cppunit/ui/text/TestRunner.h>

#include <curl/curl.h>

// Vars to store the revision numbers of the table we have updated	
static std::string gamesRev="";
static std::string usersRev="";

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
			"testGetGames",
			&GameProcTests::testGetGames ) );
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

/*static*/ void GameProcTests::CreateGames(int num)
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
		struct bdstring s;
		s.len = 0;
  		s.ptr = (char *)malloc(s.len+1);
  		if (s.ptr == NULL) {
    			fprintf(stderr, "malloc() failed\n");
    			exit(EXIT_FAILURE);
  		}
  		s.ptr[0] = '\0';		
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, BDUtils::curl_write );
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		CURLcode res = curl_easy_perform(curl);    
		if(res != CURLE_OK){
			cout << "curl failure: " << curl_easy_strerror(res)<<endl;
			return;		
		}

		std::string str(s.ptr);

		gamesRev = BDUtils::getRevision(str);
		//cout << "Games rev: " << gamesRev << endl;

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
		 
		curl_easy_cleanup(curl);
	}
	
	// These revision numbers are no longer valid - because
	// we've just deleted the database.
	gamesRev = "";
	usersRev = "";
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
	GameProcTests::CreateGames(2);

	BDGame game;
	int num = game.getNumOfGames();
	
	if(num!=2){
		// Clear up before forcing fail
		GameProcTests::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 
	}	

	GameProcTests::CreateGames(3);
	num = game.getNumOfGames();

	if(num != 3){
		// Clear up before forcing fail
		GameProcTests::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}

	GameProcTests::CreateGames(1);
	num=game.getNumOfGames();

	if(num != 1){
		// Clear up before forcing fail
		GameProcTests::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}

	cout << "cleanup.";
	// Reset everything just the way they were
	GameProcTests::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testGetGames()
{
	cout << "Test get vector of games.";
	// Setup
	
	cout << "init.";

	// Do setup - set up couchDB etc
	GameProcTests::CreateDatabase();
	GameProcTests::CreateUsers();
	GameProcTests::CreateGames(3);

	BDGame game;
	std::vector<std::string> vecGames = game.getVecOfGames();
	
	// See if we've got the right number back
	if(vecGames.size()!=3){
		// Clear up before forcing fail
		cout<<"Wrong number of games retrieved from db";		
		GameProcTests::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << vecGames.size();		
		CPPUNIT_FAIL(stream.str()); 
	}	
	
	// Now check each one of the 3 to check they are the right game names
	for(int i=0;i<vecGames.size();i++){
		ostringstream stream;
		stream << "Game" << i;
		string s( stream.str());
		if(vecGames[i].compare(s)!=0){
			cout << "Incorrect item found in vector: " << s;
			// Clear up before forcing fail
			GameProcTests::DeleteDatabase();
			CPPUNIT_FAIL("Incorrect item found in vector.");							
		}	
	}

	cout << "cleanup.";
	// Reset everything just the way they were
	GameProcTests::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testIsGameReady()
{

}

void GameProcTests::testProcessGame()
{

}
