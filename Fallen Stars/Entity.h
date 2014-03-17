#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include "Controls.h"
#include "AnimatedSprite.h"
#include "Occluder.h"

class b2Body;
class BoxWorld;
/*
 * Entity is a baseclass for all living and non grid-based objecets
 */
class Entity : public Occluder
{
	public:
		enum Facing{LEFT, RIGHT};
		enum EntityCategory{
			 ALL =			0,
			 PLAYER =		1 << 0,
			 ENEMY =		1 << 1,
			 ENEMY_CHASE =	1 << 2,
			 ENEMY_GROUND =	1 << 3,
			 ONESIDE_PLAT = 1 << 4,
			 OBJECT		  = 1 << 5,
			 ENEMY_ATTACK = 1 << 6,
			 PLAYER_SENSOR =1 << 7,
			 STREET_LIGHT  =1 << 8,
			 FLASHLIGHT	   =1 << 9,
			 DISABLED	   =1 << 10
		};
		enum SpriteOrigin
		{
			ORIGIN_CENTER, ORIGIN_UP, ORIGIN_DOWN, ORIGIN_LEFT, ORIGIN_RIGHT
		};

		//TODO Update skall få funktioner
		//TODO collisionWith måste ta emot boundingBoxes(bodys?)
		virtual ~Entity();
		virtual void render(sf::RenderTarget& renderSurface, sf::RenderStates states = sf::RenderStates::Default);
		virtual void update(sf::Time deltaTime)=0;
		virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;
		sf::Vector2f getPosition();
		void setPosition(float x,float y);
		virtual bool isAlive();
		Facing getFacing();
		void setFacing(Facing facing);
		EntityCategory getCategory();
		void destroyBody();
	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		Entity(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, Facing facing = LEFT, bool createCollisionBody = true);
		void updateSpriteOrigin(SpriteOrigin horizontal = SpriteOrigin::ORIGIN_CENTER, SpriteOrigin vertical = SpriteOrigin::ORIGIN_DOWN);
		AnimatedSprite anime;
		bool mAlive;
		b2Body* body;
		sf::FloatRect bodyBounds;
		EntityCategory entityCategory; 
	private:
		Facing currentFacing;
		EntityCategory currentCategory;
};