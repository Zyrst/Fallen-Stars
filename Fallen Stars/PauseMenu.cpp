#include "PauseMenu.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include "BaseResolution.h"
#include "ResourceCollection.h"
#include "Game.h"
#include "MainMenuState.h"
#include "PlatformState.h"

PauseMenu::PauseMenu(int id, ResourceCollection& resources, PlatformState* state):
	Menu(id, false),
	mState(state)
{
	sf::Vector2f width = sf::Vector2f((float)baseWidth, 0.0f);
	sf::Vector2f height = sf::Vector2f(0.0f, (float)baseHeight);

	sf::Texture& buttonTexture = resources.getTexture("Assets/Menu/Button.png");
	sf::Font& font = resources.getFont("Assets/Menu/24Janvier.otf");

	sf::Text textContinue("Resume", font, 30U);
	addButton(Button(RESUME, width / 2.0f + height * (0.5f - 1.0f/6.0f), buttonTexture, resources, textContinue));
	
	sf::Text textNewGame("Save and Close", font, 30U);
	addButton(Button(SAVE_CLOSE, width / 2.0f + height * (0.5f + 1.0f/6.0f), buttonTexture, resources, textNewGame));
}

void PauseMenu::buttonPressed(int id)
{
	if(id == RESUME)
	{
		mState->resumeGame();
		setEnabled(false);
	}
	if(id == SAVE_CLOSE) Game::instance()->loadNewState(new MainMenuState());
}

void PauseMenu::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(keystate == Controls::KeyState::RELEASED && action == Controls::Action::MENU) buttonPressed(RESUME); // fake buttonpress
	Menu::handleAction(action, keystate);
}

void PauseMenu::render(sf::RenderTarget& renderSurface)
{
	// Temporarily reset the view to the origin
	sf::View originalview = renderSurface.getView();
	sf::View adjustedView = originalview;
	adjustedView.setCenter(baseResolution / 2.0f);
	renderSurface.setView(adjustedView);

	// Darken the level
	sf::RectangleShape screenDimmer(Resolution::currentSimulatedResolution);
	screenDimmer.setFillColor(sf::Color(0,0,0,128));
	renderSurface.draw(screenDimmer);

	// Render the buttons
	Menu::render(renderSurface);

	// Restore the view
	renderSurface.setView(originalview);
}