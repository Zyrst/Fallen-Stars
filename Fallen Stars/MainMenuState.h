#pragma once
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Music.hpp>
#include "MainMenuDebug.h"

class MainMenuState : public State
{
public:
	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;
	
	enum Overlays {MAIN_MENU, MAIN_MENU_DEBUG, PLATFORM_SELECT, PUZZLE_SELECT};

private:
	sf::Music mMusic;
	sf::Sprite mBackground;
};