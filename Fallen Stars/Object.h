#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceCollection.h"

class Object: public Entity
{
	public:
		enum TYPE {STAR, STARDUST};
		Object(BoxWorld* world,sf::Vector2f& position, ResourceCollection& resource,TYPE type);
		void update(sf::Time deltaTime) override;
		bool getType();

private:
	TYPE mType;
	ResourceCollection& mResource;
	Animation* mStar;
		
};