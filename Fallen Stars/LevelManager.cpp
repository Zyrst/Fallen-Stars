#include "LevelManager.h"
#include "Player.h"
#include "Object.h"
#include "Shade.h"
#include "LightSolver.h"
#include "SiriusOverlay.h"

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

//Return the mapLoader so we can use it outside of the class
tmx::MapLoader& LevelManager::getMapLoader()
{
	return this->mapLoader;
}

//Adds player to the map
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

//Adds objects to the level, stars/stardust etc
void LevelManager::getObjectLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity, StatManager* stats)
{
	auto& layer = mapLoader.GetLayers();
	for (auto i : layer)
	{
		if (i.name.compare("Star") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource, Object::TYPE::STAR,stats));
			}
		}
		if (i.name.compare("StarDust") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource,Object::TYPE::STARDUST,stats));
			}
		}
		if (i.name.compare("Window") == 0)
		{
			for (auto& k : i.objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource,Object::TYPE::WINDOW,stats));
			}
		}
	}
}

//Adds enemies to the level
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

//Generate collision for the map
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

//Adds positional music and sound to the level
void LevelManager::getSoundLayer(MusicVector& musicVec,ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	auto music = new sf::Music;
	for (auto& l : layers)
	{
		if (l.name.compare("Sound") == 0)
		{
			for (auto &s : l.objects)
			{
				//Get the first property from our map container that generates from the parser and use it to open the sound file
				music->openFromFile(s.GetFirstPropertyName());
				music->setPosition(s.GetPosition().x, s.GetPosition().y,0);
				music->setVolume(0);
				music->setLoop(false);
				musicVec.push_back(music);
			}
		}
	}
}

void LevelManager::getSiriusLayer(State& state, ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	for (auto& l : layers)
	{
		if (l.name.compare("Sirius") == 0)
		{
			int id = 0;
			for (auto& object : l.objects)
			{
				sf::FloatRect bounds = object.GetAABB();
				auto& messages = object.GetPropertyMap();
				
				std::vector<std::string> siriusMessages;
				for(auto it = messages.begin(); it != messages.end(); it++)
				{
					siriusMessages.push_back(it->first);
				}
				
				state.addOverlay(new SiriusOverlay(id, resource, bounds, siriusMessages));
				id++;
			}
		}
	}
}