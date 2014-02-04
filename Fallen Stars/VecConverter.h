#pragma once
#include <Box2D\Common\b2Math.h>
#include <SFML\System.hpp>

class VecConverter
{
public:
	VecConverter(const float ratio = 32.0f);
	~VecConverter();

	b2Vec2 sfmlToB2(const sf::Vector2f& vector) const;
	sf::Vector2f b2ToSfml(const b2Vec2& vector) const;

private:
	const float ratio;
};