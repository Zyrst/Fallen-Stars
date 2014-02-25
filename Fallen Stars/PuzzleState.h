#pragma once
#include "state.h"

#include <vector>
#include "PuzzleRing.h"

class PuzzleState : public State
{
public:
	PuzzleState(std::string level, int ringCount, int steps);
	~PuzzleState();

	void load() override;
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderWindow& window) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

private:
	std::string mLevel;
	int mRingCount;
	int mSteps;

	int mSelectedRing;

	std::vector<PuzzleRing> mRings;
	sf::Sprite mBackground;

	void swapRing(int indexOffset);
};

