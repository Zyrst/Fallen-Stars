#include "Menu.h"

#include <SFML/Audio/Sound.hpp>
#include <exception>
#include "Button.h"
#include "ResourceCollection.h"
#include "SoundManager.h"

Menu::Menu(int id, ResourceCollection& resources, bool enabled):
	Overlay(id, enabled),
	buttonList(),
	selectedButton(0),
	swapSound( new sf::Sound( *resources.getSound("Assets/Sound/Cloud_main_menu_1.wav") ) ),
	blockDown(false),
	blockUp(false)
{
	swapSound->setVolume(7);
	swapSound->setPlayingOffset(sf::seconds(0.1f));
}

Menu::~Menu()
{
	for(int i=0; i<(int)buttonList.size(); i++)
	{
		delete buttonList[i];
	}
	buttonList.clear();
}

void Menu::render(sf::RenderTarget& renderSurface)
{
	renderButtons(renderSurface);
}

void Menu::renderButtons(sf::RenderTarget& renderSurface)
{
	for(Button* button : buttonList)
	{
		button->render(renderSurface);
	}
}

void Menu::addButton(const Button&  button)
{	
	buttonList.push_back(new Button(button));	

	// Always set the first button as highlighted
	if(buttonList.size() == 1) buttonList[0]->setHighlighted(true);
	
	// If the selected button is disabled, swap to the new button
	if(!buttonList[selectedButton]->isEnabled())
	{
		buttonList[selectedButton]->setHighlighted(false);
		selectedButton = buttonList.size() - 1; // the new button
		buttonList[selectedButton]->setHighlighted(true);
	}
}

Button& Menu::getButton(int id)
{
	for(Button* button : buttonList)
	{
		if(button->getID() == id)
		{
			return *button;
		}
	}
	throw std::exception(("Button with id " + std::to_string(id) + " doesn\'t exist!").c_str());
}

void Menu::cycleForward()
{
	if(buttonList.empty()) return;
	buttonList[selectedButton]->setHighlighted(false);

	// Cycle the button, but make sure that it's enabled
	int initialButton = selectedButton;
	do selectedButton = (selectedButton + 1) % buttonList.size();
	while(!buttonList[selectedButton]->isEnabled() && selectedButton != initialButton);

	buttonList[selectedButton]->setHighlighted(true);

	SoundManager::playSound(*swapSound);
}

void Menu::cycleBackwards()
{
	if(buttonList.empty()) return;
	buttonList[selectedButton]->setHighlighted(false);

	// Cycle the button, but make sure that it's enabled
	int initialButton = selectedButton;
	do selectedButton = (selectedButton + buttonList.size() - 1) % buttonList.size();
	while(!buttonList[selectedButton]->isEnabled() && selectedButton != initialButton);

	buttonList[selectedButton]->setHighlighted(true);

	SoundManager::playSound(*swapSound);
}

void Menu::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(buttonList.empty()) return;

	if(keystate == Controls::KeyState::PRESSED)
	{
		if(action == Controls::Action::DOWN && !blockDown)
		{
			cycleForward();
			blockDown = true;
		}
		if(action == Controls::Action::UP && !blockUp)
		{
			cycleBackwards();
			blockUp = true;
		}
	}

	if(keystate == Controls::KeyState::RELEASED)
	{
		if(action == Controls::Action::DOWN) blockDown = false;
		if(action == Controls::Action::UP) blockUp = false;
		
		if(action == Controls::Action::CONFIRM)
		{
			Button& activeButton = *buttonList[selectedButton];
			if(activeButton.isEnabled()) 
			{
				buttonPressed(activeButton.getID());
			}
		}
	}
}

void Menu::mousePressed(int x, int y, int state)
{
	// if(!leftkey || !mouseDown) return;

	for(Button* button : buttonList)
	{
		if(button->getBoundingBox().contains(sf::Vector2f((float)x, (float)y)))
		{
			buttonPressed(button->getID());
		}
	}
}