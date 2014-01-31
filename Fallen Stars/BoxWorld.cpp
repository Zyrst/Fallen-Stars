#include "BoxWorld.h"
#include <Box2D\Box2D.h>

BoxWorld::BoxWorld(b2Vec2 gravity)
: world(new b2World(gravity))
{ }


BoxWorld::~BoxWorld()
{ 
	delete world;
}
