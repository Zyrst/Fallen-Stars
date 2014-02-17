#include "EntityLiving.h"

EntityLiving::EntityLiving(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: Entity(world, size, position)
{
}

bool EntityLiving::isAlive()
{
	return mAlive;
}
