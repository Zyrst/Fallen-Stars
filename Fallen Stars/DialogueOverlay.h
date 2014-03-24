#pragma once
#include "Overlay.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <utility>

#include "DialogueMessage.h"

class ResourceCollection;
class PlatformState;
class BoxWorld;
class DialogueCallback;

class DialogueOverlay : public Overlay
{
public:
	DialogueOverlay(int id, ResourceCollection& resources, BoxWorld& world, PlatformState& mState);
	~DialogueOverlay();
	void update(const sf::Time& deltaTime) override;
	void render(sf::RenderTarget& renderSurface) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

	void addConversation(Conversation& conversation);
	void setActiveConversation(Conversation* conversation);
	DialogueMessage& getCurrentMessage(); 

private:
	PlatformState& mPlatformState;
	BoxWorld& mWorld;

	std::vector<Conversation*> mConversations;
	std::vector<DialogueCallback*> mCallbacks;
	Conversation* mActiveConversation;

	sf::Sprite mDialogueBoxLeft;
	sf::Sprite mDialogueBoxRight;
	
	sf::Text mMessageLeft;
	sf::Text mMessageRight;
	sf::Text mSkipLeft;
	sf::Text mSkipRight;

	sf::Sprite mStellaPortrait1;
	sf::Sprite mStellaPortrait2;
	sf::Sprite mStellaPortrait3;
	sf::Sprite mStellaPortrait4;
	sf::Sprite mStellaPortrait5;
	
	sf::Sprite mErebosPortrait1;
	sf::Sprite mErebosPortrait2;
	sf::Sprite mErebosPortrait3;
	sf::Sprite mErebosPortrait4;
	sf::Sprite mErebosPortrait5;
	sf::Sprite mErebosPortrait6;

	sf::Sprite mSiriusPortrait;
	sf::Sprite mAsteriaPortrait;

	DialogueMessage::Character mStellaExpression;
	DialogueMessage::Character mOtherCharacter;
	int mSelectedMessage;
	sf::Time timeDisplayed;

	void nextMessage();
	void updateSpritePositions(sf::Vector2f cameraOffset);
};