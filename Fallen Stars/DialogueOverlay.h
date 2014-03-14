#pragma once
#include "Overlay.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <utility>

class ResourceCollection;
class PlatformState;
class BoxWorld;
class DialogueCallback;

class DialogueOverlay : public Overlay
{
public:
	enum Character { SIRIUS, STELLA, EREBOS, ASTERIA, NONE };
	typedef std::pair<Character, std::string> Message;
	typedef std::pair<sf::FloatRect, std::vector<Message>> Conversation;

	DialogueOverlay(int id, ResourceCollection& resources, BoxWorld& world, PlatformState& mState);
	~DialogueOverlay();
	void render(sf::RenderTarget& renderSurface) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

	void addConversation(const Conversation& conversation);
	void setActiveConversation(Conversation* conversation);

private:
	PlatformState& mPlatformState;
	BoxWorld& mWorld;

	std::vector<Conversation> mConversations;
	std::vector<DialogueCallback*> mCallbacks;
	Conversation* mActiveConversation;

	sf::Sprite mDialogueBoxLeft;
	sf::Sprite mDialogueBoxRight;
	
	sf::Text mMessageLeft;
	sf::Text mMessageRight;
	sf::Text mSkipLeft;
	sf::Text mSkipRight;

	sf::Sprite mStellaPortrait;
	sf::Sprite mSiriusPortrait;
	sf::Sprite mErebosPortrait;
	sf::Sprite mAsteriaPortrait;

	Character mOtherCharacter;
	int mSelectedMessage;

	void nextMessage();
};