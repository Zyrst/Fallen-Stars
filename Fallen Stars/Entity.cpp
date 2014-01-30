#include "Entity.h"

Entity::Entity(sf::Sprite, sf::IntRect, sf::Vector2f):
	mAlive(true)
{}

bool Entity::isAlive()
{
	return mAlive;
}
void Entity::collidesWith(Entity* other1, Entity* other2)
{


}