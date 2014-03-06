#pragma once
#include <string>
#include "tmx\MapLoader.h"
#include "BoxWorld.h"
#include "Entity.h"
#include "ResourceCollection.h"
#include <SFML/Audio/Music.hpp>

class LightSolver;

/*A class for managing Levels
*Contains the tmx parser
*/
class LevelManager
{
public:
	typedef std::vector<Entity*> EntityVector;
	typedef std::vector<sf::Music*> MusicVector;
	LevelManager(std::string levelname);
	~LevelManager();
	void Load();
	tmx::MapLoader& getMapLoader();
	sf::Vector2f getPlayerLayer();
	void getStarLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity);
	void getStarDustLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity);
	void getEnemyLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity,sf::Vector2f size);
	void genCollision(BoxWorld* world, LightSolver* solver = nullptr);
	void getSoundLayer(MusicVector& music,ResourceCollection& resource);

private:
	std::string mLevel;
	tmx::MapLoader mapLoader;	
};
