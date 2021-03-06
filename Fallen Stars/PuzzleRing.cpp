#include "PuzzleRing.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "BaseResolution.h"
#include "ResourceCollection.h"
#include <SFML/System/Time.hpp>

bool leftPressed = false;
bool rightPressed = false;

PuzzleRing::PuzzleRing(ResourceCollection& resources, std::string level, int ringNum, sf::Vector2f position, int initialStep, int steps):
	mCurrentDirection(NONE),
	mNextDirection(NONE),
	mCurrentRotation((double) initialStep * 360.0 / (double) steps),
	mSteps(steps),
	mSelectedStep(initialStep),
	mRotationSpeed(0.25),
	mStepsPerMove(3),
	mHighlighted(false),
	mHighlightShader(resources.getShader("Assets/Shader/highlight.frag", sf::Shader::Type::Fragment))
{
	sf::Texture& texture = resources.getTexture("Assets/Puzzle/" + level + "/Ring" + std::to_string(ringNum) + ".png");

	mSprite.setTexture(texture);
	mSprite.setPosition(position);
	sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
	mSprite.setOrigin(position + size/2.0f - baseResolution/2.0f);

	mTargetRotation = mCurrentRotation;
}


PuzzleRing::~PuzzleRing()
{
}


void PuzzleRing::update(const sf::Time& deltaTime)
{
	if(mTargetRotation < mCurrentRotation)
	{
		mCurrentRotation -= deltaTime.asSeconds() * mRotationSpeed * 360.0;
		if(mTargetRotation >= mCurrentRotation)
		{
			setRotationFinished();
		}
	}
	if(mTargetRotation > mCurrentRotation)
	{
		mCurrentRotation += deltaTime.asSeconds() * mRotationSpeed * 360.0;
		if(mTargetRotation <= mCurrentRotation)
		{
			setRotationFinished();
		}
	}

	if(mCurrentDirection == NONE)
	{
		mCurrentDirection = mNextDirection;
		mNextDirection = NONE;
		setNewTarget(mCurrentDirection);
	}

	mSprite.setRotation((float)mCurrentRotation);
}


void PuzzleRing::render(sf::RenderTarget& renderSurface)
{
	mHighlightShader.setParameter("highlightStrength", mHighlighted ? 1.4f : 1.0f);
	mHighlightShader.setParameter("texture", sf::Shader::CurrentTexture);
	renderSurface.draw(mSprite, &mHighlightShader);
}


void PuzzleRing::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(keystate == Controls::KeyState::PRESSED && mCurrentDirection == NONE)
	{
		if(action == Controls::Action::LEFT) mNextDirection = LEFT;
		if(action == Controls::Action::RIGHT) mNextDirection = RIGHT;
	}
}

void PuzzleRing::setRotationFinished()
{
	mTargetRotation = mCurrentRotation = mSelectedStep * (360.0 / mSteps);
	mCurrentDirection = NONE;
}

void PuzzleRing::setNewTarget(RotationDirection direction)
{
	if (direction == LEFT)
	{
		mSelectedStep = (mSelectedStep - mStepsPerMove + mSteps) % mSteps;
		mTargetRotation = mCurrentRotation - mStepsPerMove * (360.0 / mSteps);
	}
	if(direction == RIGHT)
	{
		mSelectedStep = (mSelectedStep + mStepsPerMove + mSteps) % mSteps;
		mTargetRotation = mCurrentRotation + mStepsPerMove * (360.0 / mSteps);
	}
}

void PuzzleRing::setHighLighted(bool state)
{
	mHighlighted = state;
}

bool PuzzleRing::isCorrect()
{
	return mSelectedStep == 0 && mTargetRotation == mCurrentRotation;
}