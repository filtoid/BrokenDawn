#include <string>
#include <vector>
#include <map>

class BDPlayer;

class BDGame 
{
public:
	enum enGameState{
		GS_LOADING=0,
		GS_WAITING,
		GS_PROCESSING,
		GS_ERROR	
	};

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
	bool areAllPlayersReady();
private:
	void getGameFromJson( std::string json );
	std::string id;
	std::vector<BDPlayer> vecPlayers;
	std::map<std::string,bool> mapPlayerReady;
	std::vector<bool> vecReady;
	float version;
	int turn;
	enGameState state;
};
