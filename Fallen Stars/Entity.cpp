#include "Entity.h"

Entity::Entity(sf::Sprite, sf::IntRect, sf::Vector2f):
	mAlive(true)
{}

bool Entity::isAlive()
{
	return mAlive;
}