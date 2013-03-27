#include <cppunit/TestCase.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

class CouchDBTests : public CppUnit::TestFixture
{
public:
	static void setup( CppUnit::TestSuite* suite);

public:
	CouchDBTests(){}

	void testCouchDBInstalled();
	void testSecond();	
};
