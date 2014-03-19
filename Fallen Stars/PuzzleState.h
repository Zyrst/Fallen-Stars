#pragma once
#include "state.h"

#include <SFML/Audio/Music.hpp>
#include <vector>
#include "PuzzleRing.h"
#include "BaseResolution.h"


class PuzzleState : public State
{
public:
	enum Ovelays { CONSTELLATION };

	PuzzleState(std::string level, int ringCount, int steps, sf::Vector2f center = baseResolution / 2.0f);
	~PuzzleState();

	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

private:
	std::string mLevel;
	int mRingCount;
	int mSteps;
	sf::Vector2f mCenter;

	int mSelectedRing;
	bool completed;

	std::vector<PuzzleRing> mRings;
	sf::Sprite mBackground;
	sf::Music mMusic;

	void swapRing(int indexOffset);
};

