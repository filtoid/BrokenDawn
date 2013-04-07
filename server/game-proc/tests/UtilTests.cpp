#include "./TestUtils.h"
#include "./UtilTests.h"

#include "BDUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <cppunit/ui/text/TestRunner.h>

#include <curl/curl.h>

using namespace std;

UtilTests::UtilTests(){
	// Do the curl global setup
  	curl_global_init(CURL_GLOBAL_ALL);
}

UtilTests::~UtilTests(){
	// Do the curl global cleanup
  	curl_global_cleanup();
}


/*static*/ void UtilTests::setup( CppUnit::TestSuite* suite)
{
	// Add all of our tests
	suite->addTest( new CppUnit::TestCaller<UtilTests>(
			"testGetItemFromJson",
			&UtilTests::testGetItemFromJson ) );
	suite->addTest( new CppUnit::TestCaller<UtilTests>(
			"testGetVecFromString",
			&UtilTests::testGetVecFromString ) );
}

/*Test methods*/

void UtilTests::testGetItemFromJson()
{
	cout << "Test get json item from json.";
	string json = "{\"item1\":\"content1\",\"item2\":[\"content2.1\",\"content2.2\"],\"item3\":\"content3\",\"item4\":2345,\"item5\":\"content5\"}";
	// Setup
	string retStr = BDUtils::getItemFromJson(json,"item1");

	//cout << "Returned string = " << retStr << endl;

	if(retStr.compare("content1")!=0){
		ostringstream out;
		out << "Test failed: expected \"content1\" and got \"" << retStr << "\"" << endl;
		CPPUNIT_FAIL(out.str());
	}

	retStr = BDUtils::getItemFromJson(json,"item2");
	if(retStr.compare("[\"content2.1\",\"content2.2\"]")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"[\"content2.1\",\"content2.2\"]\" and got \"" << retStr << "\"";
		CPPUNIT_FAIL(out.str());	
	}
	
	retStr = BDUtils::getItemFromJson(json,"item3");
	if(retStr.compare("content3")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"content3\" and got \"" << retStr << "\"";
		CPPUNIT_FAIL(out.str());	
	}

	retStr = BDUtils::getItemFromJson(json,"item4");
	if(retStr.compare("2345")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"2345\" and got \"" << retStr << "\"";
		CPPUNIT_FAIL(out.str());	
	}

	retStr = BDUtils::getItemFromJson(json,"item5");
	if(retStr.compare("content5")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"content5\" and got \"" << retStr << "\"";
		CPPUNIT_FAIL(out.str());	
	}

	cout << "Passed\n";
}

void UtilTests::testGetVecFromString()
{
	std::string in = "[\"Item1\",\"Item2\",\"Item3\"]";
	std::vector<std::string>vec = BDUtils::getVecFromAry( in ); 

	if(vec.size()!=3){
		ostringstream out;
		out << "Test failed: vector is the wrong size " << vec.size() << "\"";
		CPPUNIT_FAIL(out.str());		
	}

	if(vec[0].compare("Item1")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"Item1\" and got \"" << vec[0] << "\"";
		CPPUNIT_FAIL(out.str());		
	}
	if(vec[1].compare("Item2")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"Item2\" and got \"" << vec[0] << "\"";
		CPPUNIT_FAIL(out.str());		
	}
	if(vec[2].compare("Item3")!=0)
	{
		ostringstream out;
		out << "Test failed: expected \"Item3\" and got \"" << vec[0] << "\"";
		CPPUNIT_FAIL(out.str());		
	}
}
