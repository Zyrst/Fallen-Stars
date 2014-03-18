#pragma once
#include "overlay.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ResourceCollection;
struct StatManager;

class HUDOverlay : public Overlay
{
public:
	HUDOverlay(int id, StatManager& stats, ResourceCollection& resources);
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;

private:
	StatManager& mStats;
	sf::Text mStarText;
	sf::Text mHPText;
	sf::Sprite mStarSprite;
	sf::Sprite mStarDustSprite;

	void updateText();
};

