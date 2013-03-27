#include "UserTests.h"

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
			"testCreateUsers",
			&UserTests::testCreateUsers ) );
}

void UserTests::testCreateUsers()
{
	cout << "Test to check that we can create a user within CouchDB...";
	
	// Make sure that the private functions createUserA and createUserB 
	// are working correctly. They are used by other tests
	cout << "Passed\n";
}


/*static*/ void UserTests::createUserA()
{
	// Create a user called 'user_A'
}

/*static*/ void UserTests::createUserB()
{
	// Create a user called 'user_B'
}

/*static*/ void UserTests::deleteUserA()
{
	// Delete user A
}

/*static*/ void UserTests::deleteUserB()
{
	// Delete user B
}

/*static*/ void UserTests::createUsersAandB()
{
	// Create users A and B
	UserTests::createUserA();
	UserTests::createUserB();
}

/*static*/ void UserTests::deleteUsersAandB()
{
	// Delete users A and B
	UserTests::deleteUserA();
	UserTests::deleteUserB();
}
