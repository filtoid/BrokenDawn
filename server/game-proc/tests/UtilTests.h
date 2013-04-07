#include <cppunit/TestCase.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

class UtilTests : public CppUnit::TestFixture
{
public:
	static void setup( CppUnit::TestSuite* suite );
	
public:
	UtilTests();
	~UtilTests();

	void testGetItemFromJson();
	void testGetVecFromString();

};
