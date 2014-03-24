#pragma once

#include <utility>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Time.hpp>

struct DialogueMessage
{
	enum Character { SIRIUS, STELLA1, STELLA2, STELLA3, STELLA4, STELLA5, EREBOS1, EREBOS2, EREBOS3, EREBOS4, EREBOS5, EREBOS6, ASTERIA, NONE };

	DialogueMessage(Character speaker, std::string message, sf::Sound sound);

	Character speaker;
	std::string message;
	sf::Sound sound;
	sf::Time duration;
};

typedef std::pair<sf::FloatRect, std::vector<DialogueMessage>> Conversation;