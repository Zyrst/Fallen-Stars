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
	mMessageLeft("", resources.getFont("Assets/Menu/24Janvier.otf"), 25),
	mMessageRight("", resources.getFont("Assets/Menu/24Janvier.otf"), 25),
	
	mSkipLeft("Press 'Enter' to skip", resources.getFont("Assets/Menu/24Janvier.otf"), 17),
	mSkipRight("Press 'Enter' to skip", resources.getFont("Assets/Menu/24Janvier.otf"), 17),
	
	mStellaPortrait1(resources.getTexture("Assets/GUI/Stella/StellaPortrait1.png")),
	mStellaPortrait2(resources.getTexture("Assets/GUI/Stella/StellaPortrait2.png")),
	mStellaPortrait3(resources.getTexture("Assets/GUI/Stella/StellaPortrait3.png")),
	mStellaPortrait4(resources.getTexture("Assets/GUI/Stella/StellaPortrait4.png")),
	mStellaPortrait5(resources.getTexture("Assets/GUI/Stella/StellaPortrait5.png")),

	mErebosPortrait1(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait1.png")),
	mErebosPortrait2(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait2.png")),
	mErebosPortrait3(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait3.png")),
	mErebosPortrait4(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait4.png")),
	mErebosPortrait5(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait5.png")),
	mErebosPortrait6(resources.getTexture("Assets/GUI/Erebos/ErebosPortrait6.png")),


	mSiriusPortrait(resources.getTexture("Assets/GUI/Sirius/SiriusPortrait.png")),
	mAsteriaPortrait(resources.getTexture("Assets/GUI/Asteria/AsteriaPortrait.png")),
	
	mSelectedMessage(0),
	mOtherCharacter(DialogueMessage::Character::NONE),
	mStellaExpression(DialogueMessage::Character::STELLA3),
	timeDisplayed(sf::seconds(0.0f))
{
	mMessageLeft.setColor(sf::Color(0,0,0,255));
	mMessageRight.setColor(sf::Color(0,0,0,255));
	mSkipLeft.setColor(sf::Color(0,0,0,255));
	mSkipRight.setColor(sf::Color(0,0,255,255));
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
	mDialogueBoxLeft.setPosition(cameraOffset.x + 210, cameraOffset.y + 15); 
	sf::FloatRect boxSize = mDialogueBoxLeft.getLocalBounds();
	sf::Vector2f simRes = Resolution::currentSimulatedResolution;
	mDialogueBoxRight.setPosition(cameraOffset.x + simRes.x - boxSize.width - 210, cameraOffset.y + 15);

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
	mStellaPortrait1.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mStellaPortrait2.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mStellaPortrait3.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mStellaPortrait4.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mStellaPortrait5.setPosition(cameraOffset.x + 20, cameraOffset.y + 20);
	mSiriusPortrait.setPosition(cameraOffset.x + simRes.x - (mSiriusPortrait.getLocalBounds().width + 20), cameraOffset.y + 20);
	mAsteriaPortrait.setPosition(cameraOffset.x + simRes.x - (mAsteriaPortrait.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait1.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait1.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait2.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait2.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait3.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait3.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait4.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait4.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait5.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait5.getLocalBounds().width + 20), cameraOffset.y + 20);
	mErebosPortrait6.setPosition(cameraOffset.x + simRes.x - (mErebosPortrait6.getLocalBounds().width + 20), cameraOffset.y + 20);
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
	if(mActiveConversation == NULL) return;

	switch(mStellaExpression)
	{
		case DialogueMessage::Character::STELLA1: renderSurface.draw(mStellaPortrait1); break;
		case DialogueMessage::Character::STELLA2: renderSurface.draw(mStellaPortrait2); break;
		case DialogueMessage::Character::STELLA3: renderSurface.draw(mStellaPortrait3); break;
		case DialogueMessage::Character::STELLA4: renderSurface.draw(mStellaPortrait4); break;
		case DialogueMessage::Character::STELLA5: renderSurface.draw(mStellaPortrait5); break;
	}

	switch(mOtherCharacter)
	{
		case DialogueMessage::Character::EREBOS1: renderSurface.draw(mErebosPortrait1); break;
		case DialogueMessage::Character::EREBOS2: renderSurface.draw(mErebosPortrait2); break;
		case DialogueMessage::Character::EREBOS3: renderSurface.draw(mErebosPortrait3); break;
		case DialogueMessage::Character::EREBOS4: renderSurface.draw(mErebosPortrait4); break;
		case DialogueMessage::Character::EREBOS5: renderSurface.draw(mErebosPortrait5); break;
		case DialogueMessage::Character::EREBOS6: renderSurface.draw(mErebosPortrait6); break;
		case DialogueMessage::Character::SIRIUS: renderSurface.draw(mSiriusPortrait); break;
		case DialogueMessage::Character::ASTERIA: renderSurface.draw(mAsteriaPortrait); break;
	}

	// Draw dialogue box (current speaker)
	DialogueMessage::Character speaker = mActiveConversation->second[mSelectedMessage].speaker;
	if(speaker == DialogueMessage::STELLA1 || speaker == DialogueMessage::STELLA2 || speaker == DialogueMessage::STELLA3 || speaker == DialogueMessage::STELLA4 || speaker == DialogueMessage::STELLA5) 
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
		setEnabled(false);
		return;
	}

	DialogueMessage::Character speaker = mActiveConversation->second[mSelectedMessage].speaker;
	if(speaker == DialogueMessage::STELLA1 || speaker == DialogueMessage::STELLA2 || speaker == DialogueMessage::STELLA3 || speaker == DialogueMessage::STELLA4 || speaker == DialogueMessage::STELLA5) 
	{
		mStellaExpression = speaker;
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
	
		DialogueMessage& message = getCurrentMessage();

		std::cout << "Character: " << message.speaker << std::endl;

		// Set up the first message
		if(!mActiveConversation->second.empty())
		{
			
			DialogueMessage::Character speaker = message.speaker;
			if(speaker == DialogueMessage::STELLA1 || speaker == DialogueMessage::STELLA2 || speaker == DialogueMessage::STELLA3 || speaker == DialogueMessage::STELLA4 || speaker == DialogueMessage::STELLA5) 
			{
				mStellaExpression = speaker;
				mMessageLeft.setString(getCurrentMessage().message);
			}
			else
			{
				mOtherCharacter = speaker;
				mStellaExpression = DialogueMessage::Character::STELLA3;
				mMessageRight.setString(getCurrentMessage().message);
			}
			
			std::cout << mActiveConversation->second[0].message << std::endl;

			getCurrentMessage().sound.play();
		}
	}
}

DialogueMessage& DialogueOverlay::getCurrentMessage()
{
	assert(mActiveConversation != NULL);
	assert(mActiveConversation->second.size() != 0);
	return mActiveConversation->second[mSelectedMessage];
}

void DialogueOverlay::addConversation(Conversation& conversation)
{
	// Add the conversation to the list
	mConversations.push_back(new Conversation(conversation));

	// Create the sensor in Box2D
	sf::FloatRect sensorBounds = conversation.first;
	b2Body* sensorBody = mWorld.createEntityBody(sf::Vector2f(sensorBounds.left + sensorBounds.width / 2.0f, sensorBounds.top), sf::Vector2f(sensorBounds.width, sensorBounds.height));

	sensorBody->GetFixtureList()->SetSensor(true);
	DialogueCallback* callback = new DialogueCallback(sensorBody->GetFixtureList(), *this, mConversations.back());
	mCallbacks.push_back(callback);

	sensorBody->SetGravityScale(0.0f);
	sensorBody->SetAwake(false);

	b2Filter fuck = sensorBody->GetFixtureList()->GetFilterData();
	fuck.categoryBits = Entity::DIALOGUE;
	fuck.maskBits = Entity::PLAYER;
	sensorBody->GetFixtureList()->SetFilterData(fuck);
}