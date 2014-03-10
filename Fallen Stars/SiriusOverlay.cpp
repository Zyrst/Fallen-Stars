#include "SiriusOverlay.h"

SiriusOverlay::SiriusOverlay(int id, ResourceCollection& resources, sf::FloatRect bounds, std::vector<std::string> messages):
	Overlay(id, false),
	bounds(bounds),
	messages(messages),
	selectedMessage(0)
{
}

void SiriusOverlay::render(sf::RenderTarget& renderSurface)
{

}

void SiriusOverlay::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(action == Controls::Action::CONFIRM && keystate == Controls::KeyState::RELEASED)
	{
		nextMessage();
	}
}

void SiriusOverlay::nextMessage()
{
	selectedMessage++;
	if(selectedMessage >= messages.size())
	{
		selectedMessage = 0;
		setEnabledState(false);
	}
}