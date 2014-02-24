#include "Occluder.h"


Occluder::Occluder(int filter)
: filterGroup(filter)
{}


Occluder::~Occluder()
{}

void Occluder::setFilterGroup(int filter)
{
	filterGroup = filter;
}

int Occluder::getFilterGroup() const
{
	return filterGroup;
}