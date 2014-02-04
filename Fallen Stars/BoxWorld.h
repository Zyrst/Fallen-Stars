#pragma once
#include <Box2D\Common\b2Math.h>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <vector>
#include "VecConverter.h"

class b2World;
class b2Body;

class DebugDraw;

namespace tmx
{
	class MapObject;
}

class BoxWorld
{
public:
	BoxWorld(const b2Vec2& gravity = b2Vec2(0, 10));
	~BoxWorld();

	//Returns the b2World instance.
	b2World* getWorld();

	//Returns the vector converter this BoxWorld is using.
	VecConverter* getVecConverter();

	//Generates a body for movable objects (entities)
	b2Body* createEntityBody(const sf::Vector2f& position, const sf::Vector2f& size);

	//Creates a static body (map collision)
	b2Body* createStaticBody(const std::vector<tmx::MapObject>& objects);

	//Call every frame with the deltaTime to simulate the world.
	void step(float deltaTime);

	//Draws all shapes of this BoxWorld.
	void drawDebug(sf::RenderWindow&);

private:
	b2World* world;
	VecConverter convert;
	DebugDraw* debugDraw;
};

