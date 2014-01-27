#include "Object.h"

Object::Object(sf::Sprite):
	mAlive(true)
{
}
Object::~Object()
{
}

void Object::update(float deltaTime)
{

}
void Object::render(sf::RenderWindow window)
{

}
bool Object::isAlive(){
	return mAlive;
}