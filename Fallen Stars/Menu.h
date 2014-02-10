#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

#include "Controls.h"
#include "Button.h"

class Menu
{
	public:
		Menu();
		virtual ~Menu() {};
		virtual void update(float deltaTime) {};
		virtual void render(sf::RenderTarget& renderSurface) = 0;
		virtual void handleAction(Controls::Action action, Controls::KeyState keystate);
		virtual void mousePressed(int x, int y, int state); // In case we implement mouse input

	protected:
		void addButton(Button button);
		void cycleForward();
		void cycleBackwards();
		virtual void buttonPressed(int id) = 0;

	private:
		std::vector<Button> buttonList;
		int selectedButton;
};
