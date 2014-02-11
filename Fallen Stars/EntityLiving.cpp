#include "EntityLiving.h"

EntityLiving::EntityLiving(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: Entity(world, size, position)
{

}

bool EntityLiving::isAlive()
{
	return mAlive;
}
void EntityLiving::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}
void EntityLiving::setVelocityX(float x)
{
	velocity.x = x;
}
void EntityLiving::setVelocityY(float y)
{
	velocity.y = y;
}