#pragma once
#include "overlay.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <string>

class ResourceCollection;

class ConstellationOverlay : public Overlay
{
public:
	enum Mode { FADE_IN, VISIBLE, FADE_OUT };

	ConstellationOverlay(int id, ResourceCollection& resources, std::string levelname);

	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;

private:
	void swapMode(Mode mode);


	sf::Sprite constellation;
	sf::Time elapsedTime;
	Mode currentMode;
};

