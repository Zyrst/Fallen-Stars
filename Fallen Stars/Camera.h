#pragma once
#include <SFML\System\Vector2.hpp>

//#include "Player.h"

class Player;
namespace sf { class RenderTarget; }
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