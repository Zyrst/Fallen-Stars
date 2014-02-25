#include "Menu.h"

Menu::Menu(int id, bool enabled):
	Overlay(id, enabled),
	buttonList(),
	selectedButton(0)
{}

void Menu::render(sf::RenderTarget& renderSurface)
{
	renderButtons(renderSurface);
}

void Menu::renderButtons(sf::RenderTarget& renderSurface)
{
	for(Button& button : buttonList)
	{
		button.render(renderSurface);
	}
}

void Menu::addButton(const Button&  button)
{	
	buttonList.push_back(button);	
	if(buttonList.size() == 1) buttonList[0].setHighlighted(true);
}

void Menu::cycleForward()
{
	if(buttonList.empty()) return;
	buttonList[selectedButton].setHighlighted(false);

	selectedButton = (selectedButton + 1) % buttonList.size();
	buttonList[selectedButton].setHighlighted(true);
}

void Menu::cycleBackwards()
{
	if(buttonList.empty()) return;
	buttonList[selectedButton].setHighlighted(false);

	selectedButton = (selectedButton + buttonList.size() - 1) % buttonList.size();
	buttonList[selectedButton].setHighlighted(true);
}

void Menu::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(buttonList.empty()) return;

	if(keystate == Controls::KeyState::PRESSED)
	{
		if(action == Controls::Action::DOWN) cycleForward();
		if(action == Controls::Action::UP) cycleBackwards();
	}

	if(keystate == Controls::KeyState::RELEASED)
	{
		if(action == Controls::Action::CONFIRM) buttonPressed(buttonList[selectedButton].getID());
	}
}

void Menu::mousePressed(int x, int y, int state)
{
	// if(!leftkey || !mouseDown) return;

	for(Button& button : buttonList)
	{
		if(button.getBoundingBox().contains(sf::Vector2f((float)x, (float)y)))
		{
			buttonPressed(button.getID());
		}
	}
}