#include "SiriusOverlay.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ResourceCollection.h"
#include "BaseResolution.h"

SiriusOverlay::SiriusOverlay(int id, ResourceCollection& resources, sf::FloatRect bounds, std::vector<Message> messages):
	Overlay(id, false),
	mBounds(bounds),
	mMessages(messages),
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
	// Set up the first message
	if(!mMessages.empty())
	{
		Character firstSpeaker = mMessages[0].first;
		if(firstSpeaker == STELLA) mMessageLeft.setString(mMessages.front().second);
		else mMessageRight.setString(mMessages[0].second);
	}

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

void SiriusOverlay::render(sf::RenderTarget& renderSurface)
{
	// Draw portraits
	renderSurface.draw(mStellaPortrait);
	switch(mOtherCharacter)
	{
		case SIRIUS: renderSurface.draw(mErebosPortrait); break;
		case EREBOS: renderSurface.draw(mSiriusPortrait); break;
		case ASTERIA: renderSurface.draw(mAsteriaPortrait); break;
	}

	// Draw dialogue box (current speaker)
	if(mMessages[mSelectedMessage].first == STELLA) renderSurface.draw(mDialogueBoxLeft);
	else renderSurface.draw(mDialogueBoxRight);


}

void SiriusOverlay::handleAction(Controls::Action action, Controls::KeyState keystate)
{
	if(action == Controls::Action::CONFIRM && keystate == Controls::KeyState::RELEASED)
	{
		nextMessage();
	}
}

void SiriusOverlay::nextMessage()
{
	mSelectedMessage++;

	if(mSelectedMessage >= (int) mMessages.size())
	{
		mSelectedMessage = 0;
		mOtherCharacter = NONE;
		setEnabledState(false);
		return;
	}

	Character speaker = mMessages[mSelectedMessage].first;
	if(speaker == STELLA) 
	{
		mMessageLeft.setString(mMessages[mSelectedMessage].second);
	}
	else
	{
		mOtherCharacter = speaker;
		mMessageRight.setString(mMessages[mSelectedMessage].second);
	}
}