#include "EntityLiving.h"

EntityLiving::EntityLiving(sf::Sprite& sprite, BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position)
: Entity(sprite, world, size, position)
{

}

void EntityLiving::update(sf::Time deltaTime)
{
	//position += velocity * deltaTime;
}
void EntityLiving::render(sf::RenderTarget& renderTarget)
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