#pragma once
#include <string>
#include "tmx\MapLoader.h"
#include <SFML/Audio/Music.hpp>

class LightSolver;
class Overlay;
class PlayformState;
struct StatManager;
class Entity;
class ResourceCollection;
class State;
class BoxWorld;

/*A class for managing Levels
*Contains the tmx parser
*/
class LevelManager
{
public:
	typedef std::vector<Entity*> EntityVector;
	typedef std::vector<sf::Music*> MusicVector;
	LevelManager(std::string levelname, ResourceCollection* resource);
	~LevelManager();
	void Load(ResourceCollection* resource);
	void Render(sf::RenderTarget& rendertarget);
	tmx::MapLoader& getMapLoader();
	sf::Vector2f getPlayerLayer();
	void getObjectLayer(ResourceCollection* resource,BoxWorld* world,EntityVector& entity,StatManager* stats);
	void getStreetlightLayer(ResourceCollection& resource, BoxWorld* world, LightSolver* solver, EntityVector& entity);
	void getEnemyLayer(ResourceCollection& resource,BoxWorld* world,EntityVector& entity,sf::Vector2f size);
	void genCollision(BoxWorld* world, LightSolver* solver = nullptr);
	void getSoundLayer(MusicVector& music,ResourceCollection& resource);
	void getDialogueLayer(State& state,ResourceCollection& resource);


private:
	std::string mLevel;
	tmx::MapLoader mapLoader;
};
