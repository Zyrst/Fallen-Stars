#include "Object.h"


void Object::update(float deltaTime)
{
	
}
void Object::render(sf::RenderTarget* renderSurface)
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