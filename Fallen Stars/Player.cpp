#include "Player.h"
#include <Box2D\Box2D.h>
void Player::update(sf::Time deltaTime)
{
}
void Player::render(sf::RenderTarget& renderTarget)
{
}
bool Player::isAlive()
{
	return mAlive;
}
void Player::setVelocityX(float x)
{
	velocity.x = x;
}
void Player::setVelocityY(float y)
{
	velocity.y = y;
}
void Player::setVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}
void Player::jump()
{
}
void Player::setFacing(mFacing face)
{
	mFace = face;
}