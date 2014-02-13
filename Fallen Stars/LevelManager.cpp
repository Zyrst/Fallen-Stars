#include "LevelManager.h"
#include <SFML\Window.hpp>
#include "Player.h"
#include <Box2D\Box2D.h>
#include "Object.h"
#include "Shade.h"

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
			auto j = i.objects;
			for ( auto k : j)
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
			auto j = i.objects;
			for (auto k : j)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Shade(resource,world,size,pos));
				
			}
		}
	}
}

void LevelManager::genCollision(BoxWorld* world)
{
	auto& layers = mapLoader.GetLayers();
	for (auto& l : layers)
	{
		if (l.name.compare("Collision") == 0)
		{
			world->createStaticBody(l.objects);
			break;
		}
	}
}