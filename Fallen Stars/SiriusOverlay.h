#pragma once
#include "Overlay.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

class ResourceCollection;

class SiriusOverlay : public Overlay
{
public:
	SiriusOverlay(int id, ResourceCollection& resources, sf::FloatRect bounds, std::vector<std::string> messages);
	void render(sf::RenderTarget& renderSurface) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

private:
	sf::FloatRect bounds;
	std::vector<std::string> messages;
	sf::Sprite dialogueBox;
	int selectedMessage;

	void nextMessage();
};