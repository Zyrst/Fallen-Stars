#include "State.h"

State::~State()
{
	for(auto it = mOverlays.begin(); it != mOverlays.end();it++)
	{
		delete *it;
	}
}

void State::addOverlay(Overlay* overlay)
{
	mOverlays.push_back(overlay);
}

void State::setOverlayEnabled(int id, bool enabled)
{
	for(auto it = mOverlays.begin(); it != mOverlays.end();it++)
	{
		Overlay* overlay = *it;
		if(overlay->getID() == id) 
		{
			overlay->setEnabledState(enabled);
		}
	}
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
	for(auto it = mOverlays.begin(); it != mOverlays.end(); it++)
	{
		Overlay* overlay = *it;
		if(overlay->isEnabled())
		{
			overlay->handleAction(action, keystate);
		}
	}
}