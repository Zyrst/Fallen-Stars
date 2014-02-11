#include "Entity.h"
#include "BoxWorld.h"
#include "Libraries/Box2D/Box2D/Box2D.h"
#include "VecConverter.h"

Entity::Entity(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& pos, Facing facing):
	mAlive(true),
	body(world->createEntityBody(pos, size)),
	anime(sf::seconds(0.1)),
	bodyBounds(pos, size),
	currentFacing(facing)
{

}
Entity::~Entity() {}
bool Entity::isAlive()
{
	return mAlive;
}
void Entity::collidesWith(Entity* other1, Entity* other2)
{
	//body->

}
sf::Vector2f Entity::getPosition()
{
	return Convert::b2ToSfml(body->GetPosition());
}
void Entity::setPosition(float x, float y)
{
	body->SetTransform(Convert::sfmlToB2(sf::Vector2f(x,y)),0);
}

Entity::Facing Entity::getFacing()
{
	return currentFacing;
}

void Entity::setFacing(Facing facing)
{
	if(facing != currentFacing) 
	{
		anime.scale(1, -1);
	}
	currentFacing = facing;
}