#pragma once
#include "Overlay.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <utility>

class ResourceCollection;

class SiriusOverlay : public Overlay
{
public:
	enum Character { SIRIUS, STELLA, EREBOS, ASTERIA, NONE };
	typedef std::pair<Character, std::string> Message;

	SiriusOverlay(int id, ResourceCollection& resources, sf::FloatRect bounds, std::vector<Message> messages);
	void render(sf::RenderTarget& renderSurface) override;
	void handleAction(Controls::Action action, Controls::KeyState keystate) override;

private:
	sf::FloatRect mBounds;
	std::vector<Message> mMessages;

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