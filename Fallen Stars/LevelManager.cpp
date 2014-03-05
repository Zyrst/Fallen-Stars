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
#include "StreetLight.h"

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

void LevelManager::getStreetlightLayer(ResourceCollection& resource, BoxWorld* world, LightSolver* solver, EntityVector& entity)
{
	auto& layers = mapLoader.GetLayers();
	for (tmx::MapLayer& l : layers)
	{
		if (l.name.compare("StreetLight") == 0)
		{
			std::cout << "====\n";
			sf::Vector2f position(0.0f, 0.0f);
			
			int scale = std::atoi(l.properties["scale"].c_str());
			sf::Vector2f size(scale, scale);

			auto& objects = l.objects;
			
			for (tmx::MapObject& o : objects)
			{
				if (o.GetName().compare("bounds") == 0)
				{
					sf::FloatRect aabb = o.GetAABB();
					position = sf::Vector2f(aabb.left, aabb.top);
				}
			}

			StreetLight* light = new StreetLight(world, solver, position, size, &resource.getTexture("Assets/Shader/streetlightmask.png"));
			entity.push_back(light);

			std::cout << "====\n";
		}
	}
}

void LevelManager::getSoundLayer(MusicVector& musicVec)
{
	auto& layers = mapLoader.GetLayers();
	auto music = new sf::Music;
	for (auto& l : layers)
	{
		if (l.name.compare("Sound") == 0)
		{
			for (auto &s : l.objects)
			{
				std::cout << s.GetPosition().x << std::endl;
				std::cout << s.GetPosition().y << std::endl;
				std::cout << s.GetType() << std::endl;
				music->openFromFile(s.GetPropertyMap());
				music->setPosition(s.GetPosition().x, s.GetPosition().y,0);
				std::cout << "Pos x:" <<  music->getPosition().x << std::endl;
				std::cout << "Pos y:" << music->getPosition().y << std::endl;
				music->setAttenuation(10.0f);
				music->setLoop(false);
				musicVec.push_back(music);
			}
		}
	}
}