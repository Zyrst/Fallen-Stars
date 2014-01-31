#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
/*
 * Entity �r en basklass f�r alla levande och icke grid-baserade objekt
 */
class Entity
{
	public:
		//TODO Update skall f� funktioner
		//TODO collisionWith m�ste ta emot boundingBoxes(bodys?)
		virtual void render(sf::RenderTarget& renderSurface)=0;
		virtual void update(float deltaTime)=0;
		virtual sf::Vector2f getPosition()=0;
		virtual void setPosition(float x,float y)=0;
		virtual bool isAlive();
		void collidesWith(Entity* other1, Entity* other2);

	protected:
		Entity(sf::Sprite sprite,/*TODO �ndra till bodys*/ sf::IntRect boundingBox, sf::Vector2f position);
		sf::Sprite sprite;
		sf::IntRect boundingBox;
		sf::Vector2f position;
		bool mAlive;
		enum mFacing{LEFT, RIGHT};
};