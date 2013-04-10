/* UserTests
 * =========
 *
 * These tests are to verify that users are created and destroyed and
 * to provide static functions for doing this to other test classes.
 * These tests DO NOT represent the mechanism that one should use to create
 * a user within the database - this should be done through the Erlang 
 * RESTful API. The creation demonstrated here is purely to make other tests
 * possible.
 */

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

class UserTests : public CppUnit::TestFixture
{
public:
	static void setup( CppUnit::TestSuite* suite);
	
public:
	UserTests();
	~UserTests();
	void testGetNumUsers();
	void testGetPlayers();
};

