#include "BoxWorld.h"
#include "debugdraw.h"
#include <Box2D\Box2D.h>
#include <tmx\MapObject.h>
#include "CallBack.h"
#include "VecConverter.h"

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

	class Clister : public b2ContactListener
	{
		virtual void BeginContact(b2Contact* contact) override
		{
			b2Fixture* fixA = contact->GetFixtureA();
			b2Fixture* fixB = contact->GetFixtureB();

			void* udA = contact->GetFixtureA()->GetUserData();
			void* udB = contact->GetFixtureB()->GetUserData();

			if (udA != nullptr)
			{
				((CallBack*) udA)->beginContact(fixB);
			}

			if (udB != nullptr)
			{
				((CallBack*) udB)->beginContact(fixA);
			}

		}

		virtual void EndContact(b2Contact* contact) override
		{
			b2Fixture* fixA = contact->GetFixtureA();
			b2Fixture* fixB = contact->GetFixtureB();

			void* udA = contact->GetFixtureA()->GetUserData();
			void* udB = contact->GetFixtureB()->GetUserData();

			if (udA != nullptr)
			{
				((CallBack*) udA)->endContact(fixB);
			}

			if (udB != nullptr)
			{
				((CallBack*) udB)->endContact(fixA);
			}
		}
	};
}

BoxWorld::BoxWorld(const b2Vec2& gravity)
: world(new b2World(gravity))
, debugDraw(nullptr)
, contactListener(new Clister())
{
	world->SetAllowSleeping(true);
	world->SetContactListener(contactListener);
}


BoxWorld::~BoxWorld()
{ 
	delete world;
	delete debugDraw;
	delete contactListener;
}

b2World* BoxWorld::getWorld()
{
	return world;
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
	b2Vec2 bpos = Convert::sfmlToB2(position);
	b2Vec2 bsize = Convert::sfmlToB2(size);

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
		b2Vec2 pos = Convert::sfmlToB2(o.GetPosition());
		auto& polyPoints = o.PolyPoints();
		
		for (int i = 0; i < 4; i++)
		{
			points[i] = Convert::sfmlToB2(polyPoints[i]) + pos;
		}

		b2PolygonShape shape;
		shape.Set(points, 4);

		body->CreateFixture(&shape, 1.0f);
	}

	return body;
}