#include "Overlay.h"



Overlay::Overlay(int id, bool enabled):
	mID(id),
	mEnabled(enabled)
{
}


Overlay::~Overlay()
{
}

void Overlay::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

int Overlay::getID()
{
	return mID;
}

bool Overlay::isEnabled()
{
	return mEnabled;
}