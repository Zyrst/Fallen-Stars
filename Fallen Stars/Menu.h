#pragma once

#include <vector>

#include "Overlay.h"

class Button;
class ResourceCollection;
namespace sf { class Sound; }

class Menu : public Overlay
{
	public:
		Menu(int id, ResourceCollection& resources, bool enabled = true);
		virtual ~Menu();

		virtual void render(sf::RenderTarget& renderSurface) override;
		virtual void handleAction(Controls::Action action, Controls::KeyState keystate) override;
		virtual void mousePressed(int x, int y, int state) override;  // In case we implement mouse input

	protected:
		void addButton(const Button& button);
		Button& getButton(int id);
		void renderButtons(sf::RenderTarget& renderSurface);
		void cycleForward();
		void cycleBackwards();
		virtual void buttonPressed(int id) = 0;

	private:
		std::vector<Button*> buttonList;
		int selectedButton;
		sf::Sound* swapSound;

		bool blockUp;
		bool blockDown;
};
