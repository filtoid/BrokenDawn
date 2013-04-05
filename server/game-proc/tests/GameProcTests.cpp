#include "./GameProcTests.h"
#include "./TestUtils.h"

#include "BDGame.h"
#include "BDUtils.h"
#include "BDPlayer.h"

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
			"testGetGames",
			&GameProcTests::testGetGames ) );
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testIsGameReady",
			&GameProcTests::testIsGameReady ) );
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testProcessGame",
			&GameProcTests::testProcessGame ) );
}

/*Test methods*/

void GameProcTests::testGetNumGames()
{
	cout << "Test get number of games.";
	// Setup
	
	cout << "init.";
	// Do setup - set up couchDB etc
	
	TestUtils::CreateDatabase();
	TestUtils::CreateUsers();
	TestUtils::CreateGames(2);

	int num = BDGame::getNumOfGames();
	
	if(num!=2){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 
	}	

	TestUtils::CreateGames(3);
	num = BDGame::getNumOfGames();

	if(num != 3){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}

	TestUtils::CreateGames(1);
	num= BDGame::getNumOfGames();

	if(num != 1){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}

	cout << "cleanup.";
	// Reset everything just the way they were
	TestUtils::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testGetGames()
{
	cout << "Test get vector of games.";
	// Setup
	
	cout << "init.";

	// Do setup - set up couchDB etc
	TestUtils::CreateDatabase();
	TestUtils::CreateUsers();
	TestUtils::CreateGames(3);

	std::vector<std::string> vecGames = BDGame::getVecOfGames();
	
	// See if we've got the right number back
	if(vecGames.size()!=3){
		// Clear up before forcing fail
		cout<<"Wrong number of games retrieved from db";		
		TestUtils::DeleteDatabase();
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
			TestUtils::DeleteDatabase();
			CPPUNIT_FAIL("Incorrect item found in vector.");							
		}	
	}

	cout << "cleanup.";
	// Reset everything just the way they were
	TestUtils::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testIsGameReady()
{
	// Loop through and check to see if the all players
	// have registered that they have finished their go
	
}

void GameProcTests::testProcessGame()
{

}
