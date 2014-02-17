#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>

#include "Controls.h"
#include "ResourceCollection.h"
#include "Overlay.h"

/* States represent the various different sections of the game. The state contatins the environment that the
 * game currently uses. The active state also takes care of all the rendering performed during the current
 * frame. When a state gets replaced, it is destroyed. */
class State
{
	public:
		virtual ~State();
		virtual void load() {}; // Load resources and set up the state
		virtual void update(const sf::Time& deltaTime) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
		virtual void handleAction(Controls::Action, Controls::KeyState) = 0;

		void addOverlay(Overlay* overlay);
		void setOverlayEnabled(int id, bool enabled);

		void updateOverlays(const sf::Time& deltaTime);
		void renderOverlays(sf::RenderTarget& renderSurface);
		void handleOverlayAction(Controls::Action, Controls::KeyState);

	protected:
		ResourceCollection mResourceCollection;

	private:
		std::vector<Overlay*> mOverlays;
};