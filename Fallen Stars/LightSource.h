#pragma once
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Drawable.hpp>


class LightSource : public sf::Drawable
{
public:
	LightSource(int width = 512, int height = 512);
	virtual ~LightSource();

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
	const sf::Color& getColor() const;

	void setPosition(const sf::Vector2f& pos);
	void setColor(const sf::Color& color);

	//Call this to clear the fbos.
	void clear();

	sf::RenderTexture* getOccluder();
	sf::RenderStates createRenderState() const;
	void calculateShadow();

	const sf::Texture& getDebugMap() const;
	const sf::Texture& getResult() const;

protected:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	void renderPass(sf::Shader* shader, sf::RenderTexture* fbo, const sf::Texture& source, const sf::Vector2f& size);

	sf::Shader *mapShader, *renderShader;
	sf::RenderTexture *occluderFBO, *shadowMapFBO, *shadowRenderFBO;
	sf::Vector2f position, size;
	sf::Color color;
};

