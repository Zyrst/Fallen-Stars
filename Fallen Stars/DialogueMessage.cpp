#include "DialogueMessage.h"

#include <SFML/Audio/SoundBuffer.hpp>

DialogueMessage::DialogueMessage(Character speaker, std::string message, sf::Sound sound):
	speaker(speaker),
	message(message),
	sound(sound)
{
	if(sound.getBuffer() != NULL)
	{
		this->duration = sound.getBuffer()->getDuration() + sf::seconds(1.0f);
	}
	else
	{
		duration = sf::seconds(5.0f);
	}
}