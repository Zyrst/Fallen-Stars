#pragma once
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include "MainMenu.h"

class MainMenuState : public State
{
public:
	~MainMenuState();
	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;

public:
	Menu* menu;
};