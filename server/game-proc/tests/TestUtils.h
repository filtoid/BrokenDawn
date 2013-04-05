/*
TestUtils - A bunch of static methods to help setup the database
in particular ways for other tests.
*/

class TestUtils
{
public:
	static void CreateDatabase();
	static void DeleteDatabase();
	static void CreateUsers();
	static void CreateGames(int num);

};
