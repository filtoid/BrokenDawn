#include <string>
#include <vector>

class BDPlayer
{
public:
	static int getNumOfPlayers();
	static std::vector<std::string> getVecOfPlayers();
	static bool process(int id);
public:
	BDPlayer(std::string id);
	void update();
private:
	std::string id;
	std::string name;
	std::string email;
};
