#pragma once
#include <Box2D\Common\b2Math.h>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <vector>

class b2World;
class b2Body;
class b2ContactListener;
class b2Fixture;

class DebugDraw;

namespace tmx
{
	class MapObject;
}

namespace BoxBounds
{
	sf::FloatRect boundsOfFixture(const b2Fixture* fix);
}

class BoxWorld
{
public:
	BoxWorld(const b2Vec2& gravity = b2Vec2(0, 10));
	~BoxWorld();

	//Returns the b2World instance.
	b2World* getWorld();

	//Generates a body for movable objects (entities)
	b2Body* createEntityBody(const sf::Vector2f& position, const sf::Vector2f& size, bool createCollisionBody = true);

	//Creates a static body (map collision)
	void createStaticBody(const std::vector<tmx::MapObject>& objects);

	//Call every frame with the deltaTime to simulate the world.
	void step(float deltaTime);

	//Draws all shapes of this BoxWorld.
	void drawDebug(sf::RenderWindow&);

	//Destroys the body and deletes all user data on fixtures.
	static void destroyBody(b2Body* body);

private:
	b2World* world;
	DebugDraw* debugDraw;
	b2ContactListener* contactListener;
};

