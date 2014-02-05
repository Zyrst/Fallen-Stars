#pragma once
#include <string>
#include "tmx\MapLoader.h"
/*A class for managing Levels
*Contains the tmx parser
*/
class LevelManager
{
public:
	LevelManager(std::string levelname);
	~LevelManager();
	void Load();
	void Render(sf::RenderTarget& rendertarget);
	tmx::MapLoader& getMapLoader();
private:

	std::string mLevel;
	tmx::MapLoader mapLoader;
	int mWidth;
	int mHeight;
};
