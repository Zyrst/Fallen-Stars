#pragma once
#include "Overlay.h"

#include <SFML/System/Time.hpp>

class LevelFadeOut : public Overlay
{
public:
	LevelFadeOut(int id);
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;
private:
	sf::Time elapsedTime;
};

