#include "ConvexOccluder.h"
#include <SFML\Graphics\RenderTarget.hpp>

ConvexOccluder::ConvexOccluder(int size)
: sf::ConvexShape(size)
{}


ConvexOccluder::~ConvexOccluder()
{}

void ConvexOccluder::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::ConvexShape sh(*this);
	target.draw(sh, states);
}
