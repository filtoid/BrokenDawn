#include <string>
#include <vector>

class BDPlayer;

class BDGame 
{
public:
	static int getNumOfGames();
	static std::vector<std::string> getVecOfGames();
	static bool isReadyToProcess(int id);
	static bool process(int id);
private:
	static std::string getItemFromDatabase(std::string db, std::string item);
public:
	BDGame(std::string gameID);
	~BDGame();
	void update();

	int getNumPlayers();
	float getVersion();
	int getTurn();
	
private:
	void getGameFromJson( std::string json );
	std::string id;
	std::vector<BDPlayer*> vecPlayers;
	float version;
	int turn;
};
