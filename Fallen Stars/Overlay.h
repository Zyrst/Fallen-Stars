#pragma once

#include "Controls.h"

namespace sf{ class Time; }
namespace sf{ class RenderTarget; }

class Overlay
{
public:
	Overlay(int id, bool enabled = true);
	virtual ~Overlay();

	virtual void update(const sf::Time& deltaTime) {};
	virtual void render(sf::RenderTarget& renderSurface) = 0;
	virtual void handleAction(Controls::Action action, Controls::KeyState keystate) {};
	virtual void mousePressed(int x, int y, int state) {}; // In case we implement mouse input

	void setEnabled(bool enabled);
	int getID();
	bool isEnabled();

private:
	int mID;
	bool mEnabled;
};