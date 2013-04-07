#include <cppunit/TestCase.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

class GameProcTests : public CppUnit::TestFixture
{
public:
	static void setup( CppUnit::TestSuite* suite );
	
public:
	GameProcTests();
	~GameProcTests();

	void testGetNumGames();
	void testGetGames();
	void testGetEachGame();
	void testIsGameReady();
	void testProcessGame();	
private:
	
};
