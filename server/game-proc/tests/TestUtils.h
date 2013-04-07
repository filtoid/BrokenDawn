/*
TestUtils - A bunch of static methods to help setup the database
in particular ways for other tests.
*/
#include <string>
class TestUtils
{
public:
	static void CreateDatabase();
	static void DeleteDatabase();

	static void CreatePlayers(int num);
	static void CreateInstanceOfPlayer( std::string name );
	static void CreateGames(int num);
	static void CreateInstanceOfGame( std::string name );

};
