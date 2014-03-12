#pragma once
#include "overlay.h"

#include "ResourceCollection.h"
class DeathOverlay : public Overlay
{
public:
	DeathOverlay(int id, ResourceCollection& resources);

	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;


	// sf::Time elapsedTime;
};

