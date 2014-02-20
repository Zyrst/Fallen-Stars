#pragma once

#include "Menu.h"
#include "ResourceCollection.h"

class MainMenu : public Menu
{
public:
	MainMenu(int id, ResourceCollection& resources);
	~MainMenu();
	void render(sf::RenderTarget& renderSurface) override;
	void buttonPressed(int id);

private:
	enum Buttons {START, PUZZLE, SETTINGS};
	sf::Sprite mBackground;
};

