#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Controls.h"

class Overlay
{
public:
	Overlay(int id, bool enabled = true);
	virtual ~Overlay();

	virtual void update(const sf::Time& deltaTime) {};
	virtual void render(sf::RenderTarget& renderSurface) = 0;
	virtual void handleAction(Controls::Action action, Controls::KeyState keystate) = 0;
	virtual void mousePressed(int x, int y, int state) {}; // In case we implement mouse input

	void setEnabledState(bool enabled);
	int getID();
	bool isEnabled();

private:
	bool mID;
	bool mEnabled;
};