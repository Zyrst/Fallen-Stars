#pragma once

#include <SFML/System/Vector2.hpp>

const int baseWidth = 1920;
const int baseHeight = 1080;

const sf::Vector2f baseResolution((float) baseWidth, (float) baseHeight);

struct Resolution
{
	static sf::Vector2f currentSimulatedResolution;
};