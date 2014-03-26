#include "CutsceneState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "Game.h"


CutsceneState::CutsceneState(std::string filename, State* nextState):
	filename(filename),
	loadFailed(false),
	nextState(nextState)
{}

CutsceneState::~CutsceneState()
{
	video.stop();
}

void CutsceneState::load()
{
	if(video.openFromFile(filename))
	{
		video.play();
	}
	else loadFailed = true;
}

void CutsceneState::update(const sf::Time& deltaTime)
{
	if(video.getStatus() == sfe::Movie::Status::Stopped)
	{
		Game::instance()->loadNewState(nextState);
	}
}

void CutsceneState::render(sf::RenderWindow& window)
{
	updateSize(window);
	window.draw(video);
}

void CutsceneState::handleAction(Controls::Action, Controls::KeyState)
{
	// Implement skip? Nah... ;)
}

void CutsceneState::updateSize(sf::RenderWindow& window)
{
	sf::View view = window.getView();
	sf::Vector2f center = view.getCenter();
	sf::Vector2f size = view.getSize();
	sf::FloatRect screenRect(center.x - size.x/2, center.y - size.y/2, size.x, size.y);
	video.resizeToFrame(static_cast<sf::IntRect>(screenRect), true);
}

std::string CutsceneState::getTypeName()
{
	return "Cutscene State";
}