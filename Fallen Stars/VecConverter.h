#pragma once
#include <Box2D\Common\b2Math.h>
#include <SFML\System.hpp>

namespace Convert
{
	b2Vec2 sfmlToB2(const sf::Vector2f& vector);
	sf::Vector2f b2ToSfml(const b2Vec2& vector);

	float sfmlToB2(float f);
	float b2Tosfml(float f);
}

class VecConverter
{
public:
	static const VecConverter& getInstance();
	~VecConverter();

	b2Vec2 sfmlToB2(const sf::Vector2f& vector) const;
	sf::Vector2f b2ToSfml(const b2Vec2& vector) const;

	float getRatio() const;

private:
	VecConverter(float ratio);
	const float ratio;
};