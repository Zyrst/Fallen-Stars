#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>

#include "Controls.h"
#include "Button.h"

class Menu
{
	public:
		Menu();
		virtual ~Menu() {};
		virtual void update(const sf::Time& deltaTime) {};
		virtual void render(sf::RenderTarget& renderSurface);
		virtual void handleAction(Controls::Action action, Controls::KeyState keystate);
		virtual void mousePressed(int x, int y, int state); // In case we implement mouse input

	protected:
		void addButton(const Button& button);
		void renderButtons(sf::RenderTarget& renderSurface);
		void cycleForward();
		void cycleBackwards();
		virtual void buttonPressed(int id) = 0;

	private:
		std::vector<Button> buttonList;
		int selectedButton;
};
