#pragma once
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Music.hpp>
#include "MainMenu.h"

class MainMenuState : public State
{
public:
	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;

private:
	enum Overlays {MAIN_MENU};
	sf::Music mMusic;
};