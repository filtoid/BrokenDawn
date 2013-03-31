#include <cppunit/TestCase.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

class GameProcTests : public CppUnit::TestFixture
{
public:
	static void setup( CppUnit::TestSuite* suite);
private:
	static void CreateDatabase();
	static void DeleteDatabase();
	static void CreateUsers();
	static void CreateGames();
public:
	GameProcTests();
	~GameProcTests();

	void testGetNumGames();
	void testGetGame();
	void testIsGameReady();
	void testProcessGame();	
};
