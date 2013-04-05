#include "./BDPlayer.h"

/*Static methods*/

/*static*/ int BDPlayer::getNumOfPlayers()
{
	return 2008;
}
	
/*static*/ std::vector<std::string> BDPlayer::getVecOfPlayers()
{
	std::vector<std::string> retVec;

	return retVec;
}

/*static*/ bool BDPlayer::process(int id)
{
	return true;
}

/*Non-static methods*/

BDPlayer::BDPlayer(std::string id)
{
	// Load from database with the given id
}

void BDPlayer::update()
{
	// Run updates to see if the database has changed
}
