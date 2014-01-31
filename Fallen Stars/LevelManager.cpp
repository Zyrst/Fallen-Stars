#include "LevelManager.h"
#include <iostream>


LevelManager::LevelManager(std::string levelname):
	mLevel(levelname)
{
}

void LevelManager::Load()
{
	tmx::MapLoader mapLoader("\Assets\Map");
	mapLoader.Load(mLevel);
	

}

void LevelManager::Render(sf::RenderTarget& rendertarget)
{
	
}