#include "Object.h"


void Object::update(float deltaTime)
{
	
}
void Object::render(sf::RenderWindow window)
{

}
bool Object::isAlive(){
	return mAlive;
}
sf::Vector2f Object::getPosition()
{
	return position;
}
void Object::setPosition(float x,float y)
{
	position.x = x;
	position.y = y;
}