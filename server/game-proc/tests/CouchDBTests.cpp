#include "CouchDBTests.h"

#include <iostream>
#include <string>
#include <sstream>

#include <cppunit/ui/text/TestRunner.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace std;

/*static*/ void CouchDBTests::setup( CppUnit::TestSuite* suite)
{
	// Add all of our tests
	suite->addTest( new CppUnit::TestCaller<CouchDBTests>(
			"testCouchDBInstalled",
			&CouchDBTests::testCouchDBInstalled ) );
	suite->addTest( new CppUnit::TestCaller<CouchDBTests>(
			"testSecond",
			&CouchDBTests::testSecond ) );

}

void CouchDBTests::testCouchDBInstalled()
{
	
	cout << "Test to check Couch DB is Installed...";

 	curlpp::Easy myRequest;

	// If we have a local instance of Couch DB installed then
	// it should respond on 5984 to give the version number
	myRequest.setOpt(curlpp::options::Url("http://127.0.0.1:5984/"));
	ostringstream os;
	os << myRequest;
	string content = os.str();	
	string expected = "{\"couchdb\":\"Welcome\",\"version\":\"1.2.0\"}\n";
	int ret = content.compare(expected);
	if(ret == 0)
		cout << "Passed\n";
	else
	{
		// Display some (hopefully) useful debug info
		cout << "FAILED!(" << ret << ")\n" << content << "is not equal to \n" << expected << endl;
		// Let the test suite know of our failure
		CPPUNIT_FAIL("Returned value from local instance of CouchDb not correct!"); 
	}

}

void CouchDBTests::testSecond()
{
	cout << "An as yet undefined Second Test...";
	CPPUNIT_ASSERT( 2 + 3 == 5);
	cout << "Passed\n";
}
