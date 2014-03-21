#include "DialogueOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Box2D\Box2D.h>

#include <iostream> // Ta bort

#include "ResourceCollection.h"
#include "BaseResolution.h"
#include "PlatformState.h"
#include "DialogueCallback.h"
#include "BoxWorld.h"

#include "Entity.h"

DialogueOverlay::DialogueOverlay(int id, ResourceCollection& resources, BoxWorld& world, PlatformState& platformState):
	Overlay(id),
	mPlatformState(platformState),
	mWorld(world),
	mActiveConversation(NULL),

	mDialogueBoxLeft(resources.getTexture("Assets/GUI/DialogueBoxLeft.png")),
	mDialogueBoxRight(resources.getTexture("Assets/GUI/DialogueBoxRight.png")),
	mMessageLeft("", resources.getFont("Assets/Menu/24Janvier.otf"), 35),
	mMessageRight("", resources.getFont("Assets/Menu/24Janvier.otf"), 35),
	
	mSkipLeft("Press 'Enter' to skip", resources.getFont("Assets/Menu/24Janvier.otf"), 20),
	mSkipRight("Press 'Enter' to skip", resources.getFont("Assets/Menu/24Janvier.otf"), 20),
	
	mStellaPortrait(resources.getTexture("Assets/GUI/StellaPortrait.png")),
	mSiriusPortrait(resources.getTexture("Assets/GUI/SiriusPortrait.png")),
	mErebosPortrait(resources.getTexture("Assets/GUI/ErebosPortrait.png")),
	mAsteriaPortrait(resources.getTexture("Assets/GUI/AsteriaPortrait.png")),
	
	mSelectedMessage(0),
	mOtherCharacter(DialogueMessage::Character::NONE),
	timeDisplayed(sf::seconds(0.0f))
{
	mMessageLeft.setColor(sf::Color(255,255,255,255));
	mMessageRight.setColor(sf::Color(255,255,255,255));
	mSkipLeft.setColor(sf::Color(255,255,255,255));
	mSkipRight.setColor(sf::Color(255,255,255,255));
}

DialogueOverlay::~DialogueOverlay()
{
	for(DialogueCallback* callback : mCallbacks)
	{
		delete callback;
	}
}

void DialogueOverlay::updateSpritePositions(sf::Vector2f cameraOffset)
{
	// Set up the dialogue box backgrounds
	mDialogueBoxLeft.setPosition(cameraOffset.x + 150, cameraOffset.y + 20); 
	sf::FloatRect boxSize = mDialogueBoxLeft.getLocalBounds();
	sf::Vector2f simRes = Resolution::currentSimulatedResolution;
	mDialogueBoxRight.setPosition(cameraOffset.x + simRes.x - boxSize.width - 150, cameraOffset.y + 20);

	// Position the messages within the dialogue box
	mMessageLeft.setPosition(mDialogueBoxLeft.getPosition().x + 50, mDialogueBoxLeft.getPosition().y + 30);
	mMessageRight.setPosition(mDialogueBoxRight.getPosition().x + 50, mDialogueBoxRight.getPosition().y + 30);

	// Position the skip texts
	sf::Vector2f leftBoxSize = sf::Vector2f(mDialogueBoxLeft.getLocalBounds().width, mDialogueBoxLeft.getLocalBounds().height);
	sf::Vector2f rightBoxSize = sf::Vector2f(mDialogueBoxRight.getLocalBounds().width, mDialogueBoxRight.getLocalBounds().height);
	sf::Vector2f leftTextSize = sf::Vector2f(mSkipLeft.getLocalBounds().width, mSkipLeft.getLocalBounds().height);
	sf::Vector2f rightTextSize = sf::Vector2f(mSkipRight.getLocalBounds().width, mSkipRight.getLocalBounds().height);
	sf::Vector2f leftOffset = mDialogueBoxLeft.getPosition() + leftBoxSize - leftTextSize;
	sf::Vector2f rightOffset = mDialogueBoxRight.getPosition() + rightBoxSize - rightTextSize;
	mSkipLeft.setPosition(leftOffset.x - 40, leftOffset.y - 40);
	mSkipRight.setPosition(rightOffset.x - 40, rightOffset.y - 40);

	// Place the portraits in their slots
	mStellaPortrait.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mSiriusPortrait.setPosition(cameraOffset.x + simRes.x - (mSiriusPortrait.getLocalBounds().width + 20), cameraOffset.y + 20);
	mAsteriaPortrait.setPosition(cameraOffset.x + simRes.x - (mAsteriaPortrait.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait.getLocalBounds().width + 20), cameraOffset.y + 20);
}


void DialogueOverlay::update(const sf::Time& deltaTime)
{
	if(mActiveConversation != NULL)
	{
		timeDisplayed += deltaTime;
		if(timeDisplayed > getCurrentMessage().duration) nextMessage();
	}
}

void DialogueOverlay::render(sf::RenderTarget& renderSurface)
{
	updateSpritePositions(renderSurface.getView().getCenter() - Resolution::currentSimulatedResolution / 2.0f);

	// Draw portraits
	renderSurface.draw(mStellaPortrait);
	if(mActiveConversation == NULL) return;

	switch(mOtherCharacter)
	{
		case DialogueMessage::Character::EREBOS: renderSurface.draw(mErebosPortrait); break;
		case DialogueMessage::Character::SIRIUS: renderSurface.draw(mSiriusPortrait); break;
		case DialogueMessage::Character::ASTERIA: renderSurface.draw(mAsteriaPortrait); break;
	}

	// Draw dialogue box (current speaker)
	if(mActiveConversation->second[mSelectedMessage].speaker == DialogueMessage::Character::STELLA)
	{
		mDialogueBoxLeft.setColor(sf::Color(128,128,128,255));
		renderSurface.draw(mDialogueBoxLeft);
		renderSurface.draw(mMessageLeft);
		renderSurface.draw(mSkipLeft);
	}
	else
	{
		mDialogueBoxRight.setColor(sf::Color(128,128,128,255));
		renderSurface.draw(mDialogueBoxRight);
		renderSurface.draw(mMessageRight);
		renderSurface.draw(mSkipRight);
	}
}

void DialogueOverlay::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(action == Controls::Action::CONFIRM && keystate == Controls::KeyState::RELEASED)
	{
		nextMessage();
	}
}

void DialogueOverlay::nextMessage()
{
	if(mActiveConversation == NULL) return;

	getCurrentMessage().sound.stop();
	timeDisplayed = sf::seconds(0.0f);

	mSelectedMessage++;

	if(mSelectedMessage >= (int) mActiveConversation->second.size())
	{
		mSelectedMessage = 0;
		mOtherCharacter = DialogueMessage::Character::NONE;
		mActiveConversation = NULL;
		return;
	}

	DialogueMessage::Character speaker = mActiveConversation->second[mSelectedMessage].speaker;
	if(speaker == DialogueMessage::Character::STELLA) 
	{
		mMessageLeft.setString(mActiveConversation->second[mSelectedMessage].message);
	}
	else
	{
		mOtherCharacter = speaker;
		mMessageRight.setString(mActiveConversation->second[mSelectedMessage].message);
	}

	getCurrentMessage().sound.play();
}

void DialogueOverlay::setActiveConversation(Conversation* conversation)
{
	if(conversation != NULL)
	{
		mActiveConversation = conversation;
		mSelectedMessage = 0;
	
		std::cout << "Character: " << getCurrentMessage().speaker << std::endl;

		// Set up the first message
		if(!mActiveConversation->second.empty())
		{
			DialogueMessage::Character speaker = getCurrentMessage().speaker;
			if(speaker == DialogueMessage::Character::STELLA) 
			{
				mMessageLeft.setString(getCurrentMessage().message);
			}
			else
			{
				mOtherCharacter = speaker;
				mMessageRight.setString(getCurrentMessage().message);
			}
			
			std::cout << mActiveConversation->second[0].message << std::endl;

			getCurrentMessage().sound.play();
		}
	}
}

DialogueMessage& DialogueOverlay::getCurrentMessage()
{
	return mActiveConversation->second[mSelectedMessage];
}

void DialogueOverlay::addConversation(Conversation& conversation)
{
	// Add the conversation to the list
	mConversations.push_back(conversation);

	// Create the sensor in Box2D
	sf::FloatRect sensorBounds = conversation.first;
	b2Body* sensorBody = mWorld.createEntityBody(sf::Vector2f(sensorBounds.left + sensorBounds.width / 2.0f, sensorBounds.top), sf::Vector2f(sensorBounds.width, sensorBounds.height));

	sensorBody->GetFixtureList()->SetSensor(true);
	DialogueCallback* callback = new DialogueCallback(sensorBody->GetFixtureList(), *this, &mConversations.back());
	mCallbacks.push_back(callback);

	sensorBody->SetGravityScale(0.0f);
	sensorBody->SetAwake(false);

	b2Filter fuck = sensorBody->GetFixtureList()->GetFilterData();
	fuck.categoryBits = Entity::DIALOGUE;
	fuck.maskBits = Entity::PLAYER;
	sensorBody->GetFixtureList()->SetFilterData(fuck);
}