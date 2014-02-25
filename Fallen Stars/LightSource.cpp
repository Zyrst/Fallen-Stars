#include "LightSource.h"
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>

namespace
{
	const std::string SHADER_DIR = "Assets/Shader/";

	sf::RenderTexture* createFBO(int width, int height)
	{
		sf::RenderTexture* fbo = new sf::RenderTexture();
		fbo->create(width, height, false);

		return fbo;
	}

	sf::Shader* createShader(const std::string& vertex, const std::string& fragment)
	{
		sf::Shader* sh = new sf::Shader();
		
		if (!sh->loadFromFile(SHADER_DIR + vertex, SHADER_DIR + fragment))
		{
			std::cerr << "Failed to load shader: [" << vertex << "], [" << fragment << "]\n";
		}

		return sh;
	}
}

LightSource::LightSource(int width, int height, int filter)
: mapShader(createShader("default.vert", "shadowMap.frag"))
, renderShader(createShader("default.vert", "shadowRender.frag"))
, occluderFBO(createFBO(width, height))
, shadowMapFBO(createFBO(width, 1))
, shadowRenderFBO(createFBO(width, height))
, position(0, 0)
, size((float)width, (float)height)
, color(sf::Color::Transparent)
, mask(nullptr)
, ownsMask(false)
, filterGroup(filter)
{ }


LightSource::~LightSource()
{
	delete mapShader;
	delete renderShader;
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
	sf::Vector2u usize = occluderFBO->getSize();
	sf::Vector2f size = sf::Vector2f((float) usize.x, (float) usize.y);
	mapShader->setParameter("resolution", size);

	renderShader->setParameter("resolution", size);

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
	sf::Vector2u usize2 = shadowMapFBO->getSize();
	sf::Vector2f size2 = sf::Vector2f((float) usize2.x, (float) usize2.y);

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
	sf::Sprite sp = sf::Sprite(getResult());
	sp.setOrigin(size.x / 2.0f, size.y / 2.0f);
	sp.setPosition(position);

	target.draw(sp, states);
}