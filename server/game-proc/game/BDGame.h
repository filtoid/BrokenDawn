#include <string>
#include <vector>

class BDGame 
{
public:
	BDGame();

	int getNumOfGames();
	std::vector<std::string> getVecOfGames();
	bool isReadyToProcess(int id);
	bool process(int id);
private:
	static std::string getItemFromDatabase(std::string db, std::string item);
};
