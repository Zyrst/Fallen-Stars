#include "State.h"

#include <cassert>
#include "SoundManager.h"

State::~State()
{
	for(auto it = mOverlays.begin(); it != mOverlays.end();it++)
	{
		delete *it;
	}
	SoundManager::clear();
}

void State::addOverlay(Overlay* overlay)
{
	mOverlays.push_back(overlay);
}

Overlay& State::getOverlay(int id)
{
	Overlay* targetOverlay = NULL;

	for(Overlay* overlay : mOverlays)
	{
		if(overlay->getID() == id) 
		{
			targetOverlay = overlay;
			break;
		}
	}

	assert(targetOverlay != NULL);
	return *targetOverlay;
}

void State::updateOverlays(const sf::Time& deltaTime)
{
	for(auto it = mOverlays.begin(); it != mOverlays.end(); it++)
	{
		Overlay* overlay = *it;
		if(overlay->isEnabled())
		{
			overlay->update(deltaTime);
		}
	}
}

void State::renderOverlays(sf::RenderTarget& renderSurface)
{
	for(auto it = mOverlays.begin(); it != mOverlays.end(); it++)
	{
		Overlay* overlay = *it;
		if(overlay->isEnabled())
		{
			overlay->render(renderSurface);
		}
	}
}

void State::handleOverlayAction(Controls::Action action, Controls::KeyState keystate)
{
	// Store the states of the overlays in case they change from a button press
	std::vector<bool> overlaysEnabled(mOverlays.size());
	for(unsigned int i = 0; i < mOverlays.size(); i++)
	{
		overlaysEnabled[i] = mOverlays[i]->isEnabled();
	}

	for(unsigned  int i = 0; i < mOverlays.size(); i++)
	{
		Overlay* overlay = mOverlays[i];
		if(overlaysEnabled[i])
		{
			overlay->handleAction(action, keystate);
		}
	}
}