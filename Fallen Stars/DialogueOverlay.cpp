#include "DialogueOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Box2D\Box2D.h>

#include "ResourceCollection.h"
#include "BaseResolution.h"
#include "PlatformState.h"
#include "DialogueCallback.h"
#include "BoxWorld.h"

#include "Entity.h"

DialogueOverlay::DialogueOverlay(int id, ResourceCollection& resources, BoxWorld& world, PlatformState& platformState):
	Overlay(id, false),
	mPlatformState(platformState),
	mWorld(world),

	mDialogueBoxLeft(resources.getTexture("Assets/Menu/DialogueBox.png")),
	mDialogueBoxRight(resources.getTexture("Assets/Menu/DialogueBox.png")),
	mMessageLeft("", resources.getFont("24Janvier.otf"), 20),
	mMessageRight("", resources.getFont("24Janvier.otf"), 20),
	
	mSkipLeft("Press 'Enter' to continue", resources.getFont("24Janvier.otf"), 12),
	mSkipRight("Press 'Enter' to continue", resources.getFont("24Janvier.otf"), 12),
	
	mStellaPortrait(resources.getTexture("Assets/Menu/StellaPortrait.png")),
	mSiriusPortrait(resources.getTexture("Assets/Menu/SiriusPortrait.png")),
	mErebosPortrait(resources.getTexture("Assets/Menu/ErebosPortrait.png")),
	mAsteriaPortrait(resources.getTexture("Assets/Menu/AsteriaPortrait.png")),
	
	mSelectedMessage(0),
	mOtherCharacter(NONE)
{
	// Set up the dialogue box backgrounds
	mDialogueBoxLeft.setPosition(150, 20);
	sf::FloatRect boxSize = mDialogueBoxLeft.getLocalBounds();
	mDialogueBoxRight.setPosition(baseWidth - boxSize.width - 150, 20);

	// Position the messages within the dialogue box
	mMessageLeft.setPosition(mDialogueBoxLeft.getPosition().x + 10, mDialogueBoxLeft.getPosition().y + 50);
	mMessageRight.setPosition(mDialogueBoxLeft.getPosition().x + 10, mDialogueBoxLeft.getPosition().y + 50);

	// Position the skip texts
	mSkipLeft.setPosition(mDialogueBoxLeft.getPosition().x + 10, mDialogueBoxLeft.getPosition().y + 50);
	mSkipRight.setPosition(mDialogueBoxRight.getPosition().x + 10, mDialogueBoxRight.getPosition().y + 200);

	// Place the portraits in their slots
	mStellaPortrait.setPosition(20, 20);
	mSiriusPortrait.setPosition(baseWidth - (mSiriusPortrait.getLocalBounds().width + 20), 20);
	mAsteriaPortrait.setPosition(baseWidth - (mAsteriaPortrait.getLocalBounds().width + 20), 20);
	mErebosPortrait.setPosition(baseWidth - (mErebosPortrait.getLocalBounds().width + 20), 20);
}

DialogueOverlay::~DialogueOverlay()
{
	for(DialogueCallback* callback : mCallbacks)
	{
		delete callback;
	}
}

void DialogueOverlay::render(sf::RenderTarget& renderSurface)
{
	// Draw portraits
	renderSurface.draw(mStellaPortrait);
	if(mActiveConversation == NULL) return;

	switch(mOtherCharacter)
	{
		case SIRIUS: renderSurface.draw(mErebosPortrait); break;
		case EREBOS: renderSurface.draw(mSiriusPortrait); break;
		case ASTERIA: renderSurface.draw(mAsteriaPortrait); break;
	}

	// Draw dialogue box (current speaker)
	if(mActiveConversation->second[mSelectedMessage].first == STELLA) renderSurface.draw(mDialogueBoxLeft);
	else renderSurface.draw(mDialogueBoxRight);


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
	mSelectedMessage++;

	if(mActiveConversation == NULL) return;

	if(mSelectedMessage >= (int) mActiveConversation->second.size())
	{
		mSelectedMessage = 0;
		mOtherCharacter = NONE;
		setEnabledState(false);
		return;
	}

	Character speaker = mActiveConversation->second[mSelectedMessage].first;
	if(speaker == STELLA) 
	{
		mMessageLeft.setString(mActiveConversation->second[mSelectedMessage].second);
	}
	else
	{
		mOtherCharacter = speaker;
		mMessageRight.setString(mActiveConversation->second[mSelectedMessage].second);
	}
}

void DialogueOverlay::addConversation(const Conversation& conversation)
{
	// Add the conversation to the list
	mConversations.push_back(conversation);

	// Create the sensor in Box2D
	{
		sf::FloatRect sensorBounds = conversation.first;
		b2Body* sensorBody = mWorld.createEntityBody(sf::Vector2f(sensorBounds.left + sensorBounds.width / 2.0f, sensorBounds.top), sf::Vector2f(sensorBounds.width, sensorBounds.height));

		sensorBody->GetFixtureList()->SetSensor(true);
		DialogueCallback* callback = new DialogueCallback(sensorBody->GetFixtureList(), *this);
		mCallbacks.push_back(callback);

		sensorBody->SetGravityScale(0.0f);
		sensorBody->SetAwake(false);

		b2Filter fuck = sensorBody->GetFixtureList()->GetFilterData();
		fuck.categoryBits = Entity::DIALOGUE;
		fuck.maskBits = Entity::PLAYER;
		sensorBody->GetFixtureList()->SetFilterData(fuck);
	}
}

void DialogueOverlay::setActiveConversation(Conversation* conversation)
{
	if(conversation != NULL)
	{
		mActiveConversation = conversation;
		mSelectedMessage = 0;
		
		// Set up the first message
		if(!mActiveConversation->second.empty())
		{
			Character firstSpeaker = mActiveConversation->second[0].first;
			if(firstSpeaker == STELLA) mMessageLeft.setString(mActiveConversation->second.front().second);
			else mMessageRight.setString(mActiveConversation->second[0].second);
		}
	}
}