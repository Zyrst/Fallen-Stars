#include "EntityLiving.h"

EntityLiving::EntityLiving(sf::Sprite):
	mAlive(true)
{
}
EntityLiving::~EntityLiving()
{
}
void EntityLiving::update(float deltaTime)
{

}
void EntityLiving::render(sf::RenderWindow window)
{

}
bool EntityLiving::isAlive(){
	return mAlive;
}