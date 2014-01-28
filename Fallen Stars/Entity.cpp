#include "Entity.h"

Entity::Entity(sf::Sprite, sf::IntRect):
	mAlive(true)
{}

bool Entity::isAlive()
{
	return mAlive;
}