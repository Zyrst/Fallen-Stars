#pragma once
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Drawable.hpp>

//Class to hold shaders. Mainly used to avoid duplicates with LightSolver
class LightShaderPair
{
public:
	LightShaderPair(sf::Shader* mapShader, sf::Shader* renderShader)
		: mapShader(mapShader)
		, renderShader(renderShader)
	{}

	~LightShaderPair()
	{
		delete mapShader;
		delete renderShader;
	}

	sf::Shader *mapShader, *renderShader;
};

class LightSource : public sf::Drawable
{
public:
	LightSource(LightShaderPair* shaders, int width = 512, int height = 512, int filterGroup = 255);
	virtual ~LightSource();

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
	const sf::Vector2f& getOffset() const;
	const sf::Color& getColor() const;
	int getFilterGroup() const;
	bool isEnabled() const;

	void setPosition(const sf::Vector2f& pos);
	void setOffset(const sf::Vector2f& offset);
	void setColor(const sf::Color& color);
	void setMask(sf::Texture* texture, bool ownsMask = false);
	void setFilterGroup(int filter);
	void setEnabled(bool enabled);
	

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
	bool enabled;
	sf::Shader *mapShader, *renderShader;
	sf::RenderTexture *occluderFBO, *shadowMapFBO, *shadowRenderFBO;
	sf::Vector2f position, size, offset;
	sf::Color color;
	sf::Texture* mask;

	bool ownsMask;

	int filterGroup;
};

