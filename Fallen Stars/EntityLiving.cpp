#include "EntityLiving.h"

EntityLiving::EntityLiving(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, Facing facing)
: Entity(world, size, position)
, currentFacing(facing)
{
}

bool EntityLiving::isAlive()
{
	return mAlive;
}
