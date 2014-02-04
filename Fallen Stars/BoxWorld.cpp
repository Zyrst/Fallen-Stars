#include "BoxWorld.h"
#include "debugdraw.h"
#include <Box2D\Box2D.h>
#include "Libraries\SFML TmxLoader\include\tmx\MapObject.h"

namespace
{
	//Assuming points is an array with 4 elements
	void genPoints(b2Vec2* points, const b2Vec2& pos, const b2Vec2& size)
	{
		points[0] = pos;
		points[1] = b2Vec2(pos.x + size.x, pos.y);
		points[2] = pos + size;
		points[3] = b2Vec2(pos.x, pos.y + size.y);
	}
}

BoxWorld::BoxWorld(const b2Vec2& gravity)
: world(new b2World(gravity))
, convert(32.0f)
, debugDraw(nullptr)
{
	world->SetAllowSleeping(true);
}


BoxWorld::~BoxWorld()
{ 
	delete world;
}

b2World* BoxWorld::getWorld()
{
	return world;
}

VecConverter* BoxWorld::getVecConverter()
{
	return &convert;
}

void BoxWorld::step(float deltaTime)
{
	world->Step(deltaTime, 6, 6);
}

void BoxWorld::drawDebug(sf::RenderWindow& wnd)
{
	if (debugDraw == nullptr)
	{
		debugDraw = new DebugDraw(wnd);
		debugDraw->SetFlags(b2Draw::e_shapeBit);
		world->SetDebugDraw(debugDraw);
	}

	world->DrawDebugData();
}

b2Body* BoxWorld::createEntityBody(const sf::Vector2f& position, const sf::Vector2f& size)
{
	//Define and convert the position and size vectors.
	b2Vec2 bpos = convert.sfmlToB2(position);
	b2Vec2 bsize = convert.sfmlToB2(size);

	//Create and set the position of the body.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(bpos.x, bpos.y);
	bodyDef.fixedRotation = true;

	b2Body* body = world->CreateBody(&bodyDef);

	//TODO: SET CENTER OF COLLISION BOX.
	b2PolygonShape shape;
	b2Vec2 points[4];

	//Generate the points of the shape.
	genPoints(points, bpos, bsize);
	shape.Set(points, 4);

	body->CreateFixture(&shape, 1.0f);

	return body;
}

b2Body* BoxWorld::createStaticBody(const std::vector<tmx::MapObject>& objects)
{
	b2BodyDef def;
	def.type = b2_staticBody;

	b2Body* body = world->CreateBody(&def);
	b2Vec2 points[4];

	//Start generating collision rectangles.
	for (auto& o : objects)
	{
		b2Vec2 pos = convert.sfmlToB2(o.GetPosition());
		auto& polyPoints = o.PolyPoints();
		
		for (int i = 0; i < 4; i++)
		{
			points[i] = convert.sfmlToB2(polyPoints[i]) + pos;
		}

		b2PolygonShape shape;
		shape.Set(points, 4);

		body->CreateFixture(&shape, 1.0f);
	}

	return body;
}