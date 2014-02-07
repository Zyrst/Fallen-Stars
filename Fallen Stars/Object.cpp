#include "Object.h"


void Object::update(sf::Time deltaTime)
{
	
}
void Object::render(sf::RenderTarget& renderSurface)
{

}
bool Object::isAlive(){
	return mAlive;
}
sf::Vector2f Object::getPosition()
{
	/* Add real positions here */
	//return sprite.getPosition();
	return sf::Vector2f(0.f,0.f);
}
void Object::setPosition(float x,float y)
{
	/*position.x = x;
	position.y = y;*/
}