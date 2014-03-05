#pragma once
#include "Entity.h"
#include <SFML\System\Vector2.hpp>
#include "LightSource.h"

class LightSolver;

class StreetLight : public Entity
{
public:
	StreetLight(BoxWorld* world, LightSolver* solver, sf::Vector2f& position, sf::Vector2f& size, sf::Texture* mask = nullptr);
	virtual ~StreetLight();

	virtual void render(sf::RenderTarget& renderSurface) override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleAction(Controls::Action action, Controls::KeyState) override;

private:
	LightSource* lightSource;
};

