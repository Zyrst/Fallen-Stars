#pragma once

#include <utility>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Time.hpp>

struct DialogueMessage
{
	enum Character { SIRIUS, STELLA, EREBOS, ASTERIA, NONE };

	DialogueMessage(Character speaker, std::string message, sf::Sound sound);

	Character speaker;
	std::string message;
	sf::Sound sound;
	sf::Time duration;
};

typedef std::pair<sf::FloatRect, std::vector<DialogueMessage>> Conversation;