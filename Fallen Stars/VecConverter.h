#pragma once
#include <Box2D\Common\b2Math.h>
#include <SFML\System.hpp>

namespace Convert
{
	b2Vec2 sfmlToB2(const sf::Vector2f& vector);
	sf::Vector2f b2ToSfml(const b2Vec2& vector);
}

class VecConverter
{
public:
	static const VecConverter& getInstance();
	~VecConverter();

	b2Vec2 sfmlToB2(const sf::Vector2f& vector) const;
	sf::Vector2f b2ToSfml(const b2Vec2& vector) const;

	const float getRatio() const;

private:
	VecConverter(const float ratio);
	const float ratio;
};