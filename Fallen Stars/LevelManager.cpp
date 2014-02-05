#include "LevelManager.h"
#include <iostream>
#include <SFML\Window.hpp>
#include "Controls.h"
LevelManager::LevelManager(std::string levelname):
	mLevel(levelname),
	mapLoader("Assets/Map")
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

tmx::MapLoader& LevelManager::getMapLoader()
{
	return this->mapLoader;
}