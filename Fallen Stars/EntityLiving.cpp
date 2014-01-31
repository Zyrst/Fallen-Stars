#include "EntityLiving.h"
void EntityLiving::update(float deltaTime)
{
	position += velocity * deltaTime;
}
void EntityLiving::render(sf::RenderTarget& renderTarget)
{

}
bool EntityLiving::isAlive()
{
	return mAlive;
}
sf::Vector2f EntityLiving::getPosition()
{
	return position;
}
void EntityLiving::setPosition(float x,float y)
{
	position.x = x;
	position.y = y;
}
void EntityLiving::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}