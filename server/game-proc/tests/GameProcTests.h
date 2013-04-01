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
	static void CreateGames(int num);
	
public:
	GameProcTests();
	~GameProcTests();

	void testGetNumGames();
	void testGetGames();
	void testIsGameReady();
	void testProcessGame();	
private:
	
};
