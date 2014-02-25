#pragma once
#include "Occluder.h"
#include <SFML\Graphics\ConvexShape.hpp>

//Simple class to allow polygons to be occluders
class ConvexOccluder : 	public sf::ConvexShape, public Occluder
{
public:
	ConvexOccluder(int size);
	virtual ~ConvexOccluder();

	//void setPoint(int index, const sf::Vector2f& point);
	//void setPosition()

private:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	//sf::ConvexShape shape;
};

