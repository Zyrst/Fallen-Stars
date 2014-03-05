#include "LevelManager.h"
#include <SFML\Window.hpp>
#include "Player.h"
#include <Box2D\Box2D.h>
#include "Object.h"
#include "Shade.h"
#include "LightSolver.h"
#include <iostream>
#include <SFML/Audio/Music.hpp>
#include <tmx/MapObject.h>

LevelManager::LevelManager(std::string levelname):
	mLevel(levelname),
	mapLoader("Assets/Map/")
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
	
				
	mapLoader.Draw(rendertarget,tmx::MapLayer::Background);
	mapLoader.Draw(rendertarget,tmx::MapLayer::Foreground);
	//mapLoader.Draw(rendertarget, tmx::MapLayer::All);
	//mapLoader.Draw(rendertarget,tmx::MapLayer::Object);

	//rendertarget.draw(mapLoader);
}

tmx::MapLoader& LevelManager::getMapLoader()
{
	return this->mapLoader;
}

sf::Vector2f LevelManager::getPlayerLayer()
{
	auto& layer = mapLoader.GetLayers();
	for(auto i : layer )
	{
		if (i.name.compare("Player") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				return pos;
			}
		}
	}
}

void LevelManager::getStarLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity)
{
	auto& layer = mapLoader.GetLayers();
	for (auto i : layer)
	{
		if (i.name.compare("Star") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource, Object::TYPE::STAR));
			}
		} 
	}
}

void LevelManager::getStarDustLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity)
{
	auto& layer = mapLoader.GetLayers();
	for (auto i : layer)
	{
		if (i.name.compare("StarDust") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource,Object::TYPE::STARDUST));
			}
		}
	}
}

void LevelManager::getEnemyLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity,sf::Vector2f size)
{
	auto& layer = mapLoader.GetLayers();
	for (auto i : layer)
	{
		if (i.name.compare("Enemy") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Shade(resource,world,size,pos));
				
			}
		}
	}
}

void LevelManager::genCollision(BoxWorld* world, LightSolver* solver)
{
	auto& layers = mapLoader.GetLayers();
	for (auto& l : layers)
	{
		if (l.name.compare("Collision") == 0)
		{
			world->createStaticBody(l.objects);
			if (solver != nullptr)
			{
				solver->addCollisionOccluders(l.objects);
			}
			break;
		}
	}
}

void LevelManager::getSoundLayer(MusicVector& musicVec,ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	auto music = new sf::Music;
	auto sound = new sf::Sound;
	for (auto& l : layers)
	{
		if (l.name.compare("Sound") == 0)
		{
			for (auto &s : l.objects)
			{
				music->openFromFile(s.GetPropertyMap());
				music->setPosition(s.GetPosition().x, s.GetPosition().y,0);
				std::cout << "Pos x:" <<  music->getPosition().x << std::endl;
				std::cout << "Pos y:" << music->getPosition().y << std::endl;
				music->setVolume(0);
				music->setLoop(false);
				musicVec.push_back(music);
			}
		}
	}
}