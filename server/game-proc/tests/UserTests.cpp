#include "UserTests.h"
#include "BDPlayer.h"
#include "TestUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <cppunit/ui/text/TestRunner.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace std;

/*static*/ void UserTests::setup( CppUnit::TestSuite* suite)
{
	// Add all of our tests
	suite->addTest( new CppUnit::TestCaller<UserTests>(
			"testGetNumUsers",
			&UserTests::testGetNumUsers ) );
	suite->addTest( new CppUnit::TestCaller<UserTests>(
			"testGetPlayers",
			&UserTests::testGetPlayers ) );			
}

/*Non-static members*/

UserTests::UserTests()
{
	// Do the curl global setup
  	curl_global_init(CURL_GLOBAL_ALL);
}

UserTests::~UserTests()
{
	// Do the curl global cleanup
  	curl_global_cleanup();
}

void UserTests::testGetNumUsers()
{
	cout << "Test get number of players...";
	cout << "init.";
	// Do setup - set up couchDB etc
	
	TestUtils::CreateDatabase();
	TestUtils::CreatePlayers(2);

	int num = BDPlayer::getNumOfPlayers();
	
	if(num!=2){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 
	}	

	TestUtils::CreatePlayers(3);
	num = BDPlayer::getNumOfPlayers();

	if(num != 3){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}

	TestUtils::CreatePlayers(1);
	num= BDPlayer::getNumOfPlayers();

	if(num != 1){
		// Clear up before forcing fail
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of games back: ";
		stream << num;		
		CPPUNIT_FAIL(stream.str()); 		
	}
	cout << "Cleanup\n";
	TestUtils::DeleteDatabase();

	// Make sure that the private functions createUserA and createUserB 
	// are working correctly. They are used by other tests
	cout << "Passed\n";
}

void UserTests::testGetPlayers()
{
	cout << "Test get vector of players.";
	// Setup
	
	cout << "init.";

	// Do setup - set up couchDB etc
	TestUtils::CreateDatabase();
	cout << "Creating players";
	TestUtils::CreatePlayers(3);

	std::vector<std::string> vecPlayers(BDPlayer::getVecOfPlayers());
	
	// See if we've got the right number back
	if(vecPlayers.size()!=3){
		// Clear up before forcing fail
		cout<<"Wrong number of players retrieved from db";		
		TestUtils::DeleteDatabase();
		ostringstream stream;
		stream << "We have got the wrong number of players back: ";
		stream << vecPlayers.size();		
		CPPUNIT_FAIL(stream.str()); 
	}	
	
	// Now check each one of the 3 to check they are the right game names
	for(int i=0;i<vecPlayers.size();i++){
		ostringstream stream;
		stream << "Player" << i;
		string s( stream.str());
		if(vecPlayers[i].compare(s)!=0){
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
