#pragma once
#include "State.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <thread>

class LoadingState : public State
{
public:
	LoadingState(State* nextState);
	
	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action, Controls::KeyState) override;

	State* getNextState();
	enum LoadProgress {NOT_LOADED, LOADING, DONE_LOADING};
	void setProgress(LoadProgress progress);

private:
	State* mState;
	LoadProgress mProgress;
	std::thread* mLoadingThread;

	sf::Sprite sprite;
	sf::Time mTime;

	static ResourceCollection mPermanentResources;
};