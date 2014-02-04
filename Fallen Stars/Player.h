#pragma once
#include "EntityLiving.h"
#include "Controls.h"

class Player : public EntityLiving
{
public:
	void render(sf::RenderTarget& renderSurface) override;
	void update(sf::Time deltaTime) override;
	void handleAction(Controls::Action action, Controls::KeyState);
	sf::Vector2f getPosition() override;
	void setPosition(float x, float y);
	void setVelocity(float x, float y);
	void jump();
	bool isAlive();
	void setFacing();
private:
	sf::Vector2f velocity;
	float mJumpStrength;
	float mMaxJump;
	mFacing mFace;
};