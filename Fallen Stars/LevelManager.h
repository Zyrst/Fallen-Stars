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
private:

	std::string mLevel;

	int mWidth;
	int mHeight;

};
