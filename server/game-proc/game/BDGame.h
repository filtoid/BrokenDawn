#include <string>

class BDGame 
{
public:
	BDGame();

	int getNumOfGames();
	bool isReadyToProcess(int id);
	bool process(int id);
private:
	static std::string getItemFromDatabase(std::string db, std::string item);
};
