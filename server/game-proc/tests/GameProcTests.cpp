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
	suite->addTest( new CppUnit::TestCaller<GameProcTests>(
			"testGetEachGame",
			&GameProcTests::testGetEachGame ) );	
}

/*Test methods*/

void GameProcTests::testGetNumGames()
{
	cout << "Test get number of games.";
	// Setup
	
	cout << "init.";
	// Do setup - set up couchDB etc
	
	TestUtils::CreateDatabase();
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
	TestUtils::CreateGames(3);

	std::vector<std::string> vecGames(BDGame::getVecOfGames());
	
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

void GameProcTests::testGetEachGame()
{
	// Get the games that have been created
	cout << "Test each game created and loads.";
	// Setup
	
	cout << "init.";

	// Do setup - set up couchDB etc
	TestUtils::CreateDatabase();
	TestUtils::CreateGames(3);
	
	std::vector<std::string> vecGamesStr(BDGame::getVecOfGames());

	// Now check each one of the 3 to check they are the right game names
	
	for(int i=0;i<vecGamesStr.size();i++){
						
		BDGame game(vecGamesStr[i]);

		// Get information from game and check against what we
		// created.
		if(game.getVersion()!=0.1f){
			cout<<"Failed to get the right version (" << game.getVersion() << ")" << endl;
			CPPUNIT_FAIL("Failed to get the right version number");		
		}
		
		if(game.getNumPlayers()!=2){
			cout<<"Failed to get the right number of players (" << game.getNumPlayers() << ")" << endl;
			CPPUNIT_FAIL("Failed to get the right number of players");		
		}	
		
		if(game.getTurn()!=26){
			cout<<"Failed to get the right turn number (" << game.getTurn() << ")" << endl;
			CPPUNIT_FAIL("Failed to get the right number of players");		
		}
	
	}

	cout << "cleanup.";
	// Reset everything just the way they were
	TestUtils::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testIsGameReady()
{

	cout << "Test if the games are ready for processing.";
	cout << ".init";
	TestUtils::CreateDatabase();
	TestUtils::CreateGames(2,false); // Create games but not instances - we will do that below
	
	std::vector<std::string> vecGamesStr(BDGame::getVecOfGames());

	// Create the game
	TestUtils::CreateInstanceOfGame(vecGamesStr[0],false);
	TestUtils::CreateInstanceOfGame(vecGamesStr[1],true);
	
	{
		BDGame game(vecGamesStr[0]);
		if(game.areAllPlayersReady()!=false){
			TestUtils::DeleteDatabase();		
			CPPUNIT_FAIL("The game reported it was ready when in fact it was not");	
		}
	}

	{
		BDGame game(vecGamesStr[1]);
		if(game.areAllPlayersReady()!=true){
			TestUtils::DeleteDatabase();
			CPPUNIT_FAIL("The game reported it was not ready when in fact it was");	
		}
	}


	cout << "cleanup.";
	// Reset everything just the way they were
	TestUtils::DeleteDatabase();

	cout << "Passed\n";
}

void GameProcTests::testProcessGame()
{

}
