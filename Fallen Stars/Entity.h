#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include "Controls.h"
#include "AnimatedSprite.h"

class b2Body;
class BoxWorld;
/*
 * Entity är en basklass för alla levande och icke grid-baserade objekt
 */
class Entity
{
	public:
		enum Facing{LEFT, RIGHT};
		enum EntityCategory{
			 ALL =			0x0000,
			 PLAYER =		0x0001,
			 ENEMY =		0x0002,
			 ENEMY_CHASE =	0x0004,
			 ENEMY_GROUND =	0x0008
		};
		//TODO Update skall få funktioner
		//TODO collisionWith måste ta emot boundingBoxes(bodys?)
		virtual ~Entity();
		void render(sf::RenderTarget& renderSurface);
		virtual void update(sf::Time deltaTime)=0;
		virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;
		sf::Vector2f getPosition();
		void setPosition(float x,float y);
		virtual bool isAlive();
		Facing getFacing();
		void setFacing(Facing facing);

	protected:
		Entity(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position, Facing facing = LEFT);
		void updateSpriteOrigin();
		AnimatedSprite anime;
		bool mAlive;
		b2Body* body;
		sf::FloatRect bodyBounds;
		EntityCategory entityCategory; 
	private:
		Facing currentFacing;
};