#pragma once
#include <string>
#include "Libraries\TmxParser\Tmx.h"

/*A class for managing Levels
*Contains the tmx parser
*/
class LevelManager
{
public:
	LevelManager(std::string levelname);
	~LevelManager();
	void Load();
protected:
	Tmx::Map* map;
	int mWidth;
	int mHeight;
};
