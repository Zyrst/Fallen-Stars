#pragma once

#include <vector>
#include "Button.h"
#include "Overlay.h"

class Menu : public Overlay
{
	public:
		Menu(int mID);
		virtual ~Menu() {};

		virtual void render(sf::RenderTarget& renderSurface) override;
		virtual void handleAction(Controls::Action action, Controls::KeyState keystate) override;
		virtual void mousePressed(int x, int y, int state) override;  // In case we implement mouse input

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
