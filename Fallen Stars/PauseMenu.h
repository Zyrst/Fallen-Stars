#pragma once
#include "Menu.h"

namespace sf { class Sound; }
class ResourceCollection;
class PlatformState;

class PauseMenu : public Menu
{
public:
	PauseMenu(int id, ResourceCollection& resources, PlatformState* state);
	void buttonPressed(int id) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;
	void render(sf::RenderTarget& renderSurface) override;

private:
	enum Buttons {RESUME, SAVE_CLOSE};
	PlatformState* mState;
};
