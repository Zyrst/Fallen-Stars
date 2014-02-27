#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <string>

#include "Controls.h"

namespace sf{ class Time; }
class ResourceCollection;

class PuzzleRing
{
public:
	enum RotationDirection {LEFT, RIGHT, NONE};

	PuzzleRing(ResourceCollection& resources, std::string level, int ringNum, sf::Vector2f position, int initialStep, int steps);
	~PuzzleRing();

	void update(const sf::Time& deltaTime);
	void render(sf::RenderTarget& renderSurface);
	void handleAction(Controls::Action action, Controls::KeyState keystate);
	
	void setHighLighted(bool state);
	bool isCorrect();

private:
	sf::Sprite mSprite;
	sf::Shader& mHighlightShader;

	RotationDirection mCurrentDirection;
	RotationDirection mNextDirection;
	double mCurrentRotation;
	double mTargetRotation;
	double mRotationSpeed;

	int mSteps;
	int mSelectedStep;
	int mStepsPerMove;
	bool mHighlighted;

	void setRotationFinished();
	void setNewTarget(RotationDirection direction);
};