#include "./BDPlayer.h"
#include "./BDUtils.h"
/*Static methods*/

/*static*/ std::vector<std::string> BDPlayer::getVecOfPlayers()
{
	// Get the whole table
	std::string playersTable = BDUtils::getItemFromDatabase("http://127.0.0.1:5984/test_db","bdplayers");
	playersTable = playersTable.substr(0,playersTable.find("}")); // Scrub any uknown chars	
		
	// Get just the array of data we want
	std::string playerList = BDUtils::getItemFromJson(playersTable,"players");

 	return BDUtils::getVecFromAry(playerList) ;
}

/*static*/ int BDPlayer::getNumOfPlayers()
{
	return getVecOfPlayers().size();
}

/*static*/ bool BDPlayer::process(int id)
{
	return true;
}

/*Non-static methods*/

BDPlayer::BDPlayer(std::string pid)
:	id(pid)
{
	// Load from database with the given id

}

void BDPlayer::update()
{
	// Run updates to see if the database has changed
}
