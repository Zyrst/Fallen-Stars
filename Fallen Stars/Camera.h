#pragma once
#include <SFML\Window.hpp>
#include <SFML\System\Vector2.hpp>
#include "Player.h"

class Camera
{
public:
	Camera(Player* player,sf::Vector2u bounds);
	~Camera();
	void update(sf::RenderTarget& target);
private:
	Player* mPlayer;
	sf::Vector2u mBounds;

};