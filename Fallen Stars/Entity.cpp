#include "Entity.h"

Entity::Entity():
	mAlive(true)
{}

bool Entity::isAlive()
{
	return mAlive;
}