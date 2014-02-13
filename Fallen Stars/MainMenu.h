#pragma once

#include "Menu.h"
#include "ResourceCollection.h"

class MainMenu : public Menu
{
public:
	MainMenu(ResourceCollection& resources);
	~MainMenu();
	virtual void render(sf::RenderTarget& renderSurface);
	void buttonPressed(int id);

private:
	enum Buttons {START};
	sf::Sprite mBackground;
};

