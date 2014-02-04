#include "LevelManager.h"
#include <iostream>
#include <SFML\Window.hpp>

tmx::MapLoader mapLoader("Assets/Map");

LevelManager::LevelManager(std::string levelname):
	mLevel(levelname)
{
	LevelManager::Load();
}

LevelManager::~LevelManager()
{
}

void LevelManager::Load()
{
	mapLoader.Load(mLevel + ".tmx");
}

void LevelManager::Render(sf::RenderTarget& rendertarget)
{
	
			//mapLoader.Draw(rendertarget,tmx::MapLayer::Background);
			//mapLoader.Draw(rendertarget,tmx::MapLayer::Foreground);
			mapLoader.Draw(rendertarget,tmx::MapLayer::Object);

	//rendertarget.draw(mapLoader);
}