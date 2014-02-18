#include "ShaderTestState.h"
#include <iostream>
#include <SFML\Graphics.hpp>

using namespace std;
using namespace sf;

namespace
{
	sf::Shader *occluderShader, *shadowMap, *shadowRender;
	sf::Texture* texture;
	sf::Sprite* sp;

	sf::RenderTexture *occluderFBO, *shadowMapFBO, *shadowRenderFBO;

	sf::RenderTexture* createFBO(int width, int height)
	{
		sf::RenderTexture* tx = new sf::RenderTexture();
		tx->create(width, height, false);

		return tx;
	}

	void renderToOccluder()
	{
		occluderFBO->clear(sf::Color::Transparent);
		occluderFBO->draw(*sp, occluderShader);
		occluderFBO->display();
	}

	void renderToShadowMap()
	{
		sf::Vector2u usize = shadowMapFBO->getSize();
		sf::Vector2f size = sf::Vector2f((float) usize.x, (float) usize.y);
		shadowMap->setParameter("resolution", size);

		shadowMapFBO->clear(sf::Color::Transparent);

		shadowMapFBO->draw(sf::Sprite(occluderFBO->getTexture()), shadowMap);

		shadowMapFBO->display();
	}

	void renderToShadowRender()
	{
		sf::Vector2u usize = shadowRenderFBO->getSize();
		sf::Vector2f size = sf::Vector2f((float)usize.x, (float)usize.y);
		shadowRender->setParameter("resolution", size);

		shadowRenderFBO->clear(sf::Color::Transparent);

		shadowRenderFBO->draw(sf::Sprite(shadowMapFBO->getTexture()), shadowRender);

		shadowRenderFBO->display();
	}
}

ShaderTestState::ShaderTestState()
{
	shadowMap = new Shader();
	shadowRender = new Shader();
	occluderShader = new Shader();

	if (!shadowMap->loadFromFile("Assets/Shaders/testvert.c", "Assets/Shaders/shadowMap.frag"))
	{
		cerr << "No loading shader0" << endl;
	}
	else if (!shadowRender->loadFromFile("Assets/Shaders/testvert.c", "Assets/Shaders/shadowRender.frag"))
	{
		cerr << "No loading shader1" << endl;
	}
	else if (!occluderShader->loadFromFile("Assets/Shaders/testvert.c", "Assets/Shaders/testfrag.c"))
	{
		cerr << "No loading shader2" << endl;
	}
	else
	{
		cout << "Woho!" << endl;
	}

	texture = new sf::Texture();
	texture->loadFromFile("Assets/Shaders/hej.png");

	sp = new sf::Sprite(*texture);

	occluderFBO = createFBO(512, 512);
	shadowMapFBO = createFBO(512, 512);
	shadowRenderFBO = createFBO(512, 512);
}


ShaderTestState::~ShaderTestState()
{}

void ShaderTestState::update(const sf::Time& deltaTime)
{

}

void ShaderTestState::render(sf::RenderWindow& window)
{
	renderToOccluder();
	renderToShadowMap();
	renderToShadowRender();

	window.clear(sf::Color::Blue);

	sf::Sprite hej = sf::Sprite(occluderFBO->getTexture());

	window.draw(hej);

	sf::Sprite hej2 = sf::Sprite(shadowMapFBO->getTexture());
	hej2.setPosition(0, hej.getLocalBounds().height);
	window.draw(hej2);

	sf::Sprite hej3 = sf::Sprite(shadowRenderFBO->getTexture());
	hej3.setPosition(hej.getLocalBounds().width, 0);
	window.draw(hej3);

	sp->setPosition(hej.getLocalBounds().width, 0);
	window.draw(*sp);

	sp->setPosition(0, 0);
}

void ShaderTestState::handleAction(Controls::Action, Controls::KeyState)
{

}