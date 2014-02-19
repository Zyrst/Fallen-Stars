#include "ShaderTestState.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include "LightSource.h"
#include "LevelManager.h"
#include "BoxWorld.h"
#include "Entity.h"
#include "ResourceCollection.h"

using namespace std;
using namespace sf;

namespace
{
	bool upB = false, downB = false, rightB = false, leftB = false;
	float speed = 400.0f;
	LevelManager* level;
	BoxWorld* world;
	LevelManager::EntityVector entities;
	ResourceCollection rc;

	sf::Shader *occluderShader, *shadowMap, *shadowRender;
	sf::Texture* texture;
	sf::Sprite* sp;

	sf::RenderTexture *occluderFBO, *shadowMapFBO, *shadowRenderFBO;

	LightSource* light;

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
		sf::Vector2u usize = occluderFBO->getSize();
		sf::Vector2f size = sf::Vector2f((float) usize.x, (float) usize.y);
		shadowMap->setParameter("resolution", size);

		sf::Vector2u usize2 = shadowMapFBO->getSize();
		sf::Vector2f size2 = sf::Vector2f((float) usize2.x, (float) usize2.y);
		sf::RectangleShape shape = sf::RectangleShape(size2);
		shape.setTexture(&(occluderFBO->getTexture()));

		shadowMapFBO->clear(sf::Color::Transparent);

		//shadowMapFBO->draw(sf::Sprite(occluderFBO->getTexture()), shadowMap);
		shadowMapFBO->draw(shape, shadowMap);

		shadowMapFBO->display();
	}

	void renderToShadowRender()
	{
		sf::Vector2u usize = shadowRenderFBO->getSize();
		sf::Vector2f size = sf::Vector2f((float)usize.x, (float)usize.y);
		shadowRender->setParameter("resolution", size);

		shadowRenderFBO->clear(sf::Color::Transparent);
		sf::RectangleShape shape = sf::RectangleShape(size);
		shape.setTexture(&(shadowMapFBO->getTexture()));
		shadowRenderFBO->draw(shape, shadowRender);

		shadowRenderFBO->display();
	}
}

ShaderTestState::ShaderTestState()
{
	/*shadowMap = new Shader();
	shadowRender = new Shader();
	occluderShader = new Shader();

	if (!shadowMap->loadFromFile("Assets/Shaders/default.vert", "Assets/Shaders/shadowMap.frag"))
	{
		cerr << "No loading shader0" << endl;
	}
	else if (!shadowRender->loadFromFile("Assets/Shaders/default.vert", "Assets/Shaders/shadowRender.frag"))
	{
		cerr << "No loading shader1" << endl;
	}
	else if (!occluderShader->loadFromFile("Assets/Shaders/default.vert", "Assets/Shaders/occluder.frag"))
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
	shadowMapFBO = createFBO(512, 1);
	shadowRenderFBO = createFBO(512, 512);*/

	light = new LightSource(512, 512);
	level = new LevelManager("Test");

	world = new BoxWorld();
	level->genCollision(world);
	//level->getEnemyLayer(rc, world, entities, sf::Vector2f(70, 220));
	level->getStarLayer(rc, world, entities);
	level->getStarDustLayer(rc, world, entities);
}


ShaderTestState::~ShaderTestState()
{}

void ShaderTestState::update(const sf::Time& deltaTime)
{
	sf::Vector2f pos = light->getPosition();
	if (leftB)
	{
		pos.x -= speed * deltaTime.asSeconds();
	}
	else if (rightB)
	{
		pos.x += speed * deltaTime.asSeconds();
	}

	if (upB)
	{
		pos.y -= speed * deltaTime.asSeconds();
	}
	else if (downB)
	{
		pos.y += speed * deltaTime.asSeconds();
	}

	light->setPosition(pos);

	world->step(deltaTime.asSeconds());
}

void ShaderTestState::render(sf::RenderWindow& window)
{
	/*renderToOccluder();
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

	sp->setPosition(0, 0);*/

	level->Render(window);

	for (Entity* e : entities)
	{
		e->render(window);
	}

	world->drawDebug(window);

	sf::RectangleShape shape = sf::RectangleShape(light->getSize());
	shape.setOrigin(light->getSize().x / 2.0f, light->getSize().y / 2.0f);
	shape.setPosition(light->getPosition());
	shape.setOutlineColor(sf::Color::Red);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);

	window.draw(shape);

	light->clear();

	sf::RenderTexture* tx = light->getOccluder();
	sf::View view = tx->getView();
	view.setCenter(light->getPosition());
	tx->setView(view);
	for (Entity* e : entities)
	{
		e->render(*tx);
	}

	light->calculateShadow();
	window.draw(*light);

	/*sf::Sprite spriteLol = sf::Sprite(light->getDebugMap());
	sf::RectangleShape shape2 = sf::RectangleShape(sf::Vector2f(512, 512));
	shape2.setFillColor(sf::Color::White);
	window.draw(shape2);
	window.draw(spriteLol);*/

}

void ShaderTestState::handleAction(Controls::Action action, Controls::KeyState state)
{
	bool keyDown = (state == Controls::KeyState::HELD);

	switch (action)
	{
	case Controls::Action::LEFT:
		leftB = keyDown;
		break;
	case Controls::Action::RIGHT:
		rightB = keyDown;
		break;
	case Controls::Action::DOWN:
		downB = keyDown;
		break;
	case Controls::Action::UP:
		upB = keyDown;
		break;
	}
}