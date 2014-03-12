#include "LevelManager.h"
#include "Player.h"
#include "Object.h"
#include "Shade.h"
#include "LightSolver.h"
#include "SiriusOverlay.h"
#include <tmx/MapObject.h>
#include "StreetLight.h"

LevelManager::LevelManager(std::string levelname, ResourceCollection* resource):
	mLevel(levelname),
	mapLoader("Assets/Map/")
{
	LevelManager::Load(resource);
}

LevelManager::~LevelManager()
{
}

void LevelManager::Load(ResourceCollection* resource)
{
	mapLoader.Load(mLevel + ".tmx", resource);
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
	for(auto& i : layer )
	{
		if (i->name.compare("Player") == 0)
		{
			for (auto& k : i->objects)
			{
				auto pos = k.GetPosition();
				return pos;
			}
		}
	}
}

//Adds objects to the level, stars/stardust etc
void LevelManager::getObjectLayer(ResourceCollection* resource,BoxWorld* world,EntityVector& entity, StatManager* stats)
{
	auto& layer = mapLoader.GetLayers();
	for (auto& i : layer)
	{
		if (i->name.compare("Star") == 0)
		{
			for (auto& k : i->objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource, Object::TYPE::STAR,stats));
			}
		}
		if (i->name.compare("StarDust") == 0)
		{
			for (auto& k : i->objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,pos,resource,Object::TYPE::STARDUST,stats));
			}
		}
		if (i->name.compare("Window") == 0)
		{
			for (auto& k : i->objects)
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
	for (auto& i : layer)
	{
		if (i->name.compare("Enemy") == 0)
		{
			for (auto& k : i->objects)
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
		if (l->name.compare("Collision") == 0)
		{
			world->createStaticBody(l->objects);
			if (solver != nullptr)
			{
				solver->addCollisionOccluders(l->objects);
			}
			break;
		}
	}
}

void LevelManager::getStreetlightLayer(ResourceCollection& resource, BoxWorld* world, LightSolver* solver, EntityVector& entity)
{
	auto& layers = mapLoader.GetLayers();
	for (tmx::MapLayer* l : layers)
	{
		if (l->name.compare("StreetLight") == 0)
		{
			sf::Vector2f position(0.0f, 0.0f);
			sf::FloatRect sensor(0.0f, 0.0f, 100.0f, 100.0f);
			
			int width = std::atoi(l->properties["width"].c_str());
			int height = std::atoi(l->properties["height"].c_str());
			sf::Vector2f size(width, height);

			auto& objects = l->objects;
			
			for (tmx::MapObject& o : objects)
			{
				if (o.GetName().compare("position") == 0)
				{
					sf::FloatRect aabb = o.GetAABB();
					position = sf::Vector2f(aabb.left + aabb.width/2.0f, aabb.top);
				}
				else if (o.GetName().compare("sensor") == 0)
				{
					sensor = o.GetAABB();
				}
			}

			StreetLight* light = new StreetLight(world, solver, position, size, sensor, &resource.getTexture("Assets/Shader/streetlightmask.png"));
			entity.push_back(light);
		}
	}
}

//Adds positional music and sound to the level
void LevelManager::getSoundLayer(MusicVector& musicVec,ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	for (auto& l : layers)
	{
		if (l->name.compare("Sound") == 0)
		{
			for (auto &s : l->objects)
			{
				auto music = new sf::Music;
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
	for (tmx::MapLayer* l : layers)
	{
		if (l->name.compare("Text") == 0)
		{
			int id = 0;
			for (tmx::MapObject& object : l->objects)
			{
				sf::FloatRect bounds = object.GetAABB();
				std::map<std::string, std::string>& messages = object.GetPropertyMap();
				
				std::vector<SiriusOverlay::Message> siriusMessages;
				for(auto it = messages.begin(); it != messages.end(); it++)
				{
					SiriusOverlay::Character speaker;
					std::string name = it->first;
					
					if		(name == "Stella") speaker = SiriusOverlay::Character::STELLA;
					else if (name == "Erebos") speaker = SiriusOverlay::Character::EREBOS;
					else if (name == "Sirius") speaker = SiriusOverlay::Character::SIRIUS;
					else if (name == "Asteria") speaker = SiriusOverlay::Character::ASTERIA;
					else	continue; // If the name doesn't match any character, skip this message
					
					siriusMessages.push_back(SiriusOverlay::Message(speaker, it->second));
				}
				
				state.addOverlay(new SiriusOverlay(id, resource, bounds, siriusMessages));
				id++;
			}
		}
	}
}