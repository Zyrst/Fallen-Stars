#include "LevelManager.h"
#include "Entity.h"
#include "Player.h"
#include "Object.h"
#include "Shade.h"
#include "LightSolver.h"
#include "DialogueOverlay.h"
#include <tmx/MapObject.h>
#include "StreetLight.h"
#include "PlatformState.h"
#include "DialogueOverlay.h"
#include "StatManager.h"
#include "ResourceCollection.h"
#include "State.h"
#include "BoxWorld.h"
#include "AnimatedBackgroundImage.h"


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
	return sf::Vector2f(); // Player position missing
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
				entity.push_back(new Object(world,sf::Vector2f(100,100),pos,resource, Object::TYPE::STAR,stats));
				stats->totalStars++;
			}
		}
		if (i->name.compare("StarDust") == 0)
		{
			for (auto& k : i->objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,sf::Vector2f(100,100),pos,resource,Object::TYPE::STARDUST,stats));
			}
		}
		if (i->name.compare("Window") == 0)
		{
			for (auto& k : i->objects)
			{
				auto pos = k.GetPosition();
				entity.push_back(new Object(world,sf::Vector2f(300, 287),pos,resource,Object::TYPE::WINDOW,stats));
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

			if (width == 0)
			{
				width = 800;
				std::cout << "Warning: Width of streetlight was undefined, setting it to " << width << std::endl;
			}
			
			if (height == 0)
			{
				height = 1024;
				std::cout << "Warning: height of streetlight was undefined, setting it to " << height << std::endl;
			}

			sf::Vector2f size((float) width, (float) height);

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

void LevelManager::getDialogueLayer(State& state, ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	for (tmx::MapLayer* l : layers)
	{
		if (l->name.compare("Text") == 0)
		{
			DialogueOverlay* dialogueOverlay = &dynamic_cast<DialogueOverlay&> (state.getOverlay(PlatformState::DIALOGUE));
			assert(dialogueOverlay != NULL);

			for (tmx::MapObject& object : l->objects)
			{
				sf::FloatRect bounds = object.GetAABB();
				std::map<std::string, std::string>& messages = object.GetPropertyMap();
				
				std::vector<DialogueOverlay::Message> siriusMessages;
				for(auto it = messages.begin(); it != messages.end(); it++)
				{
					DialogueOverlay::Character speaker;
					std::string name = it->first;
					
					if		(name == "Stella") speaker = DialogueOverlay::Character::STELLA;
					else if (name == "Erebos") speaker = DialogueOverlay::Character::EREBOS;
					else if (name == "Sirius") speaker = DialogueOverlay::Character::SIRIUS;
					else if (name == "Asteria") speaker = DialogueOverlay::Character::ASTERIA;
					else	continue; // If the name doesn't match any character, skip this message
					
					auto itNext = it;
					if((itNext++)->first == "Sound")
					{
						it++;
						// Add sound
					}

					siriusMessages.push_back(DialogueOverlay::Message(speaker, it->second));
				}
				
				dialogueOverlay->addConversation(DialogueOverlay::Conversation(bounds, siriusMessages));
			}
		}
	}
}

void LevelManager::getAnimationLayer(AnimationVector animations, ResourceCollection& resource)
{
	auto& layers = mapLoader.GetLayers();
	for (tmx::MapLayer* l : layers)
	{
		if (l->name.compare("Animation") == 0)
		{
			for (tmx::MapObject& object : l->objects)
			{
				sf::FloatRect box = object.GetAABB();
				sf::Vector2f position(box.left, box.top);
				sf::Vector2i spriteSize((int)box.width, (int)box.height);
				
				int gid = object.getGID();
				int tilesetID = mapLoader.getTileSetID(gid);
				const sf::Texture& texture = l->layerSets[tilesetID]->getTexture();

				animations.push_back(new AnimatedBackgroundImage(position, spriteSize, texture));
			}
		}
	}
}