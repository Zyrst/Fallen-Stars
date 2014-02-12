#pragma once
#include "State.h"
#include "Entity.h"
class JumpingTest :	public State
{
public:
	JumpingTest();
	virtual ~JumpingTest();

	virtual void update(const sf::Time& deltaTime) override;
	virtual void render(sf::RenderWindow& window) override;
	virtual void handleAction(Controls::Action action, Controls::KeyState) override;
	typedef std::vector<Entity*> EntityVector;
private:
	EntityVector mEntityVector;
};

