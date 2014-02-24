#pragma once
#include <SFML\Graphics\Drawable.hpp>

//This class is used by the light-engine to filter
//and draw occluders that will be used for generating shadows.
class Occluder : public sf::Drawable
{
public:
	Occluder(int filterGroup = 255);
	virtual ~Occluder();

	void setFilterGroup(int filter);
	int getFilterGroup() const;

protected:
	//It's up to the subclass to define this function.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

	int filterGroup;
};