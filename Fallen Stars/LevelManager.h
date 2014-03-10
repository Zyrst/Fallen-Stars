#pragma once
#include <string>
#include "tmx\MapLoader.h"
#include "BoxWorld.h"
#include "Entity.h"
#include "ResourceCollection.h"
#include "State.h"
#include <SFML/Audio/Music.hpp>
#include "StatManager.h"

class LightSolver;
class Overlay;

/*A class for managing Levels
*Contains the tmx parser
*/
class LevelManager
{
public:
	typedef std::vector<Entity*> EntityVector;
	typedef std::vector<sf::Music*> MusicVector;
	typedef std::vector<Overlay*> OverlayVector;
	LevelManager(std::string levelname);
	~LevelManager();
	void Load();
	tmx::MapLoader& getMapLoader();
	sf::Vector2f getPlayerLayer();
	void getObjectLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity,StatManager* stats);
	void getEnemyLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity,sf::Vector2f size);
	void genCollision(BoxWorld* world, LightSolver* solver = nullptr);
	void getSoundLayer(MusicVector& music,ResourceCollection& resource);
	void getSiriusLayer(State& state,ResourceCollection& resource);

private:
	std::string mLevel;
	tmx::MapLoader mapLoader;
};
