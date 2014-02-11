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
		//TODO Update skall få funktioner
		//TODO collisionWith måste ta emot boundingBoxes(bodys?)
		virtual ~Entity();
		virtual void render(sf::RenderTarget& renderSurface)=0;
		virtual void update(sf::Time deltaTime)=0;
		virtual void handleAction(Controls::Action action, Controls::KeyState) = 0;
		virtual sf::Vector2f getPosition();
		virtual void setPosition(float x,float y);
		virtual bool isAlive();
		void collidesWith(Entity* other1, Entity* other2);

	protected:
		Entity(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& position);
		AnimatedSprite anime;
		bool mAlive;
		enum Facing{LEFT, RIGHT};
		b2Body* body;
		sf::FloatRect bodyBounds;
};