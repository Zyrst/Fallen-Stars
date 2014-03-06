#include "LightSource.h"
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Graphics\Shader.hpp>
#include "Game.h"

namespace
{
	class FBORuntimeEvent : public RuntimeEvent
	{
	public:
		FBORuntimeEvent(sf::RenderTexture* fbo, int width, int height)
			: fbo(fbo) 
			, width(width)
			, height(height)
		{ }

		virtual void run()
		{
			fbo->create(width, height, false);
		}

	private:
		int width, height;
		sf::RenderTexture* fbo;
	};

	sf::RenderTexture* createFBO(int width, int height)
	{
		sf::RenderTexture* fbo = new sf::RenderTexture();
		
		Game::instance()->postRuntimeEvent(new FBORuntimeEvent(fbo, width, height));

		return fbo;
	}
}

LightSource::LightSource(LightShaderPair* shaders, int width, int height, float scale, int filter)
: enabled(true)
, mapShader(shaders->mapShader)
, renderShader(shaders->renderShader)
, occluderFBO(createFBO(width, height))
, shadowMapFBO(createFBO(width, 1))
, shadowRenderFBO(createFBO(width, height))
, position(0, 0)
, size((float)width, (float)height)
, color(sf::Color::Transparent)
, mask(nullptr)
, ownsMask(false)
, filterGroup(filter)
, scale(scale)
{ }


LightSource::~LightSource()
{
	delete occluderFBO;
	delete shadowMapFBO;
	delete shadowRenderFBO;

	if (ownsMask)
	{
		delete mask;
	}
}

const sf::Vector2f& LightSource::getPosition() const
{
	return position;
}

const sf::Vector2f& LightSource::getSize() const
{
	return size;
}

const sf::Color& LightSource::getColor() const
{
	return color;
}

int LightSource::getFilterGroup() const
{
	return filterGroup;
}

float LightSource::getScale() const
{
	return scale;
}

bool LightSource::isEnabled() const
{
	return enabled;
}

void LightSource::setPosition(const sf::Vector2f& pos)
{
	this->position = pos;
}

void LightSource::setColor(const sf::Color& color)
{
	this->color = color;
}

void LightSource::setMask(sf::Texture* mask, bool ownsMask)
{
	if (this->ownsMask)
	{
		delete this->mask;
	}

	this->mask = mask;
	this->ownsMask = ownsMask;
}

void LightSource::setFilterGroup(int filter)
{
	filterGroup = filter;
}

void LightSource::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

void LightSource::clear()
{
	occluderFBO->clear(sf::Color::Transparent);
	shadowMapFBO->clear(sf::Color::Transparent);
	shadowRenderFBO->clear(sf::Color::Transparent);
}

sf::RenderTexture* LightSource::getOccluder()
{
	return occluderFBO;
}

sf::RenderStates LightSource::createRenderState() const
{
	sf::RenderStates st;
	st.transform.translate(position.x - size.x / 2.0f, position.y - size.y / 2.0f);
	return st;
}

void LightSource::calculateShadow()
{
	occluderFBO->display();

	//Get the size of the main buffers.
	const sf::Vector2f& size = this->size;
	mapShader->setParameter("resolution", size);
	//mapShader->setParameter("upScale", scale);

	//renderShader->setParameter("resolution", size);

	if (mask == nullptr)
	{
		renderShader->setParameter("useMask", 0.0f);
	}
	else
	{
		renderShader->setParameter("useMask", 1.0f);
		renderShader->setParameter("mask", *mask);
	}

	//Create a shape which will render the shadowmap
	sf::Vector2f size2 = (sf::Vector2f) shadowMapFBO->getSize();

	//First pass, calculate the shadowmap.
	renderPass(mapShader, shadowMapFBO, occluderFBO->getTexture(), size2);

	//Second pass, draw the light.
	renderPass(renderShader, shadowRenderFBO, shadowMapFBO->getTexture(), size);
}

void LightSource::renderPass(sf::Shader* shader, sf::RenderTexture* fbo, const sf::Texture& source, const sf::Vector2f& size)
{
	sf::RectangleShape shape = sf::RectangleShape(size);
	shape.setTexture(&source, false);

	fbo->clear(sf::Color::Transparent);
	fbo->draw(shape, shader);
	fbo->display();
}

const sf::Texture& LightSource::getResult() const
{
	return shadowRenderFBO->getTexture();
}

const sf::Texture& LightSource::getDebugMap() const
{
	return shadowMapFBO->getTexture();
}

void LightSource::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::Vector2f size = this->size*(scale*scale);
	sf::RectangleShape sp = sf::RectangleShape(size);
	sp.setTexture(&getResult());
	sp.setOrigin(size.x / 2.0f, size.y / 2.0f);
	sp.setPosition(position);

	target.draw(sp, states);
}