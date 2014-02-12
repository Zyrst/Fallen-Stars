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
		TYPE getType();
		void handleAction(Controls::Action action, Controls::KeyState);
		void render(sf::RenderTarget& target);

private:
	TYPE mType;
	ResourceCollection& mResource;
	Animation* mStar;
	Animation* mStarDust;
		
};