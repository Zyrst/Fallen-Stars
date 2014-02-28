#include "BoxWorld.h"
#include "debugdraw.h"
#include <Box2D\Box2D.h>
#include <tmx\MapObject.h>
#include "CallBack.h"
#include "VecConverter.h"

#include <iostream>

namespace BoxBounds
{
	sf::FloatRect boundsOfFixture(const b2Fixture* fix)
	{
		using namespace std;

		//xy1 = min, xy2 = max
		float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

		const b2Shape* shape = fix->GetShape();

		//Make sure this fixture is a polygon
		if (shape->GetType() == b2Shape::e_polygon)
		{
			const b2PolygonShape* sh = (b2PolygonShape*) shape;
			
			for (int i = 0; i < sh->GetVertexCount(); i++)
			{
				const b2Vec2& vert = sh->GetVertex(i);

				//If this is the first vertex, initialize all coordinates.
				if (i == 0)
				{
					x1 = x2 = vert.x;
					y1 = y2 = vert.y;
				}
				else
				{
					//Update xy1 and xy2 accordingly to the minimum and maximum coordinates of the fixture.
					x1 = min(vert.x, x1);
					x2 = max(vert.x, x2);
					y1 = min(vert.y, y1);
					y2 = max(vert.y, y2);
				}
			}
		}

		//Since we are returning an sf::FloatRect, we should convert the coordinates to
		//sfml coordinate space.
		const b2Vec2& bodyPos = fix->GetBody()->GetPosition();
		sf::Vector2f pos = Convert::b2ToSfml(b2Vec2(x1+bodyPos.x, y1+bodyPos.y));
		sf::Vector2f size = Convert::b2ToSfml(b2Vec2(x2 - x1, y2 - y1));

		return sf::FloatRect(pos, size);
	}
}

namespace
{
	//Assuming points is an array with 4 elements
	void genPoints(b2Vec2* points, const b2Vec2& size)
	{
		const float hw = size.x / 2.0f;
		points[0] = b2Vec2(-hw, 0);
		points[1] = b2Vec2(size.x-hw, 0);
		points[2] = b2Vec2(size.x-hw, size.y);
		points[3] = b2Vec2(-hw,  size.y);
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
	world->Step(deltaTime, 50, 60);
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
	genPoints(points, bsize);
	shape.Set(points, 4);

	body->CreateFixture(&shape, 1.0f);

	return body;
}

void BoxWorld::createStaticBody(const std::vector<tmx::MapObject>& objects)
{
	b2BodyDef def;
	def.type = b2_staticBody;
	b2Vec2 points[4];
	//Start generating collision rectangles.
	for (auto& o : objects)
	{
		b2Body* body = world->CreateBody(&def);

		b2Vec2 pos = Convert::sfmlToB2(o.GetPosition());
		body->SetTransform(pos, 0.0f);

		std::cout << "Pos: " << o.GetPosition().x << ":" << o.GetPosition().y << "\n";

		auto& polyPoints = o.PolyPoints();
		
		for (int i = 0; i < 4; i++)
		{
			points[i] = Convert::sfmlToB2(polyPoints[i]);
			std::cout << "point: " << polyPoints[i].x << ":" << polyPoints[i].y << "\n";
		}

		std::cout << "\n";

		b2PolygonShape shape;
		shape.Set(points, 4);

		body->CreateFixture(&shape, 1.0f);
	}
}