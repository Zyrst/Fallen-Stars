#pragma once
#include "EntityLiving.h"

class Player : public EntityLiving
{
public:
	void render(sf::RenderTarget& renderSurface) override;
	void update(sf::Time deltaTime) override;
	sf::Vector2f getPosition() override;
	void setPosition(float x, float y);
	void setVelocityX(float x);
	void setVelocityY(float y);
	void setVelocity(float x, float y);
	void jump();
	bool isAlive();
	void setFacing(mFacing face);
private:
	sf::Vector2f velocity;
	mFacing mFace;
};