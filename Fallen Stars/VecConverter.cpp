#include "VecConverter.h"

namespace Convert
{
	b2Vec2 sfmlToB2(const sf::Vector2f& vector)
	{
		return VecConverter::getInstance().sfmlToB2(vector);
	}

	sf::Vector2f b2ToSfml(const b2Vec2& vector)
	{
		return VecConverter::getInstance().b2ToSfml(vector);
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

const VecConverter& VecConverter::getInstance()
{
	static const VecConverter cv(150.0f);

	return cv;
}

const float VecConverter::getRatio() const
{
	return this->ratio;
}