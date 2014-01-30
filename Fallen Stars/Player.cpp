#include "Player.h"

void Player::update(float deltaTime)
{
	position += velocity * deltaTime;
}
void Player::render(sf::RenderTarget& renderTarget)
{

}
bool Player::isAlive()
{
	return mAlive;
}
sf::Vector2f Player::getPosition()
{
	return position;
}
void Player::setPosition(float x,float y)
{
	position.x = x;
	position.y = y;
}
void Player::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}
void Player::jump()
{
	/*
	if(velocity.y = 0){
	velocity.y += mJumpStrength;
	if(velocity.y <= mMaxJump){
		velocity.y -= mJumpStrength;
	}
	}
	else{ velocity.y -=mJumpStrength;}
	*/
}