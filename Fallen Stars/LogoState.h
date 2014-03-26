#pragma once

#include "state.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

namespace sf { class RenderWindow; }

class LogoState : public State
{
public:
	LogoState();
	~LogoState();

	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;
	
	std::string getTypeName() override;

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::Time mTime;
};

