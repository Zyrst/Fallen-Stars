#include "VecConverter.h"

static VecConverter cv(32.0f);

namespace Convert
{
	b2Vec2 sfmlToB2(const sf::Vector2f& vector)
	{
		return cv.sfmlToB2(vector);
	}

	sf::Vector2f b2ToSfml(const b2Vec2& vector)
	{
		return cv.b2ToSfml(vector);
	}
}

VecConverter::VecConverter(const float ratio)
: ratio(ratio)
{ }


VecConverter::~VecConverter()
{ }

b2Vec2 VecConverter::sfmlToB2(const sf::Vector2f& vector) const
{
	return b2Vec2(vector.x / ratio, vector.y / ratio);
}

sf::Vector2f VecConverter::b2ToSfml(const b2Vec2& vector) const
{
	return sf::Vector2f(vector.x * ratio, vector.y * ratio);
}