#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf { class Shader; }
namespace sf { class Texture; }
namespace sf { class RenderTarget; }

class ResourceCollection;

class Button
{
public:
	Button(int id, sf::Vector2f position, sf::Texture& texture, ResourceCollection& resources, sf::Text text = sf::Text(), bool enabled = true, float textOffset = 0.0f, sf::IntRect textureRect = sf::IntRect());
	~Button();
	void render(sf::RenderTarget& renderSurface);
	void setHighlighted(bool state);
	bool isEnabled();
	int getID();
	sf::FloatRect getBoundingBox();

private:
	sf::Text mText;
	sf::Vector2f mPosition;
	sf::Sprite mSprite;
	sf::Shader& mHighlightShader;
	bool mHighlighted;
	bool mEnabled;
	int mID;
};