#pragma once
#include "overlay.h"

#include <SFML/System/Time.hpp>
#include "ResourceCollection.h"

class PlatformState;
namespace sf { class RenderTarget; }

class DeathOverlay : public Overlay
{
public:
	enum Mode { FADE_OUT, BLINK, SWARMED, DARK, FADE_IN, DONE};

	DeathOverlay(int id, ResourceCollection& resources, PlatformState& platformState);

	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;

private:
	void swapMode(Mode mode);
	void reset();

	sf::Time elapsedTime;
	Mode currentMode;
	PlatformState& platformState;
};

