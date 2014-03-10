#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

#include "Menu.h"
#include "ResourceCollection.h"

class Album : public Menu
{
public:
	Album(int id, ResourceCollection resources);
	~Album();

	sf::Sprite mAlbum;
	sf::Sprite mLeftArrow;
	sf::Sprite mRightArrow;
	sf::Sprite mBackButton;
	std::vector<sf::Sprite> mCards;
};