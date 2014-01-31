#pragma once
#include <Box2D\Common\b2Math.h>
class b2World;

class BoxWorld
{
public:
	BoxWorld(b2Vec2 gravity = b2Vec2(0, 10));
	~BoxWorld();

private:
	b2World* world;
};

