#include "LevelManager.h"
#include <iostream>
#include <SFML\Window.hpp>
#include "Player.h"
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
	//Player* player;
				
	mapLoader.Draw(rendertarget,tmx::MapLayer::Background);
//	player->render(rendertarget);
	mapLoader.Draw(rendertarget,tmx::MapLayer::Foreground);
	//mapLoader.Draw(rendertarget, tmx::MapLayer::All);
	//mapLoader.Draw(rendertarget,tmx::MapLayer::Object);

	//rendertarget.draw(mapLoader);
}

tmx::MapLoader& LevelManager::getMapLoader()
{
	return this->mapLoader;
}