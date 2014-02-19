#include "ShaderTestState.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include "LightSource.h"
#include "LevelManager.h"
#include "BoxWorld.h"
#include "Entity.h"
#include "ResourceCollection.h"
#include "BaseResolution.h"

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

	Shader *sh, *sh2;
	LightSource *light, *light2;

	RenderTexture *darkness1, *darkness2;

	RenderTexture* createFBO(int width, int height)
	{
		RenderTexture* tx = new RenderTexture();
		tx->create(width, height);
		return tx;
	}

	Shader* createShader(const string& vert, const string& frag)
	{
		Shader* shader = new Shader();
		shader->loadFromFile(vert, frag);
		return shader;
	}

	void fixcolorlight()
	{
		darkness2->clear(Color::Transparent);
		RenderStates states = RenderStates(BlendMode::BlendAdd);
		{
			RectangleShape rect = RectangleShape(light->getSize());
			rect.setOrigin(light->getSize().x / 2.0f, light->getSize().y / 2.0f);
			rect.setFillColor(light->getColor());
			rect.setPosition(light->getPosition());
			rect.setTexture(&light->getResult());
			darkness2->draw(rect, states);
		}
		{
			RectangleShape rect = RectangleShape(light2->getSize());
			rect.setOrigin(light2->getSize().x / 2.0f, light2->getSize().y / 2.0f);
			rect.setFillColor(light2->getColor());
			rect.setPosition(light2->getPosition());
			rect.setTexture(&light2->getResult());
			darkness2->draw(rect, states);
		}
		darkness2->display();
	}
}

ShaderTestState::ShaderTestState()
{
	light = new LightSource(1024, 1024);
	light2 = new LightSource(512, 512);

	light->setColor(Color(255, 255, 0, 40));
	light2->setPosition(sf::Vector2f(820, 200));
	light2->setColor(Color(255, 0, 0, 50));

	level = new LevelManager("Test");
	darkness1 = createFBO(baseWidth, baseHeight);
	darkness2 = createFBO(baseWidth, baseHeight);

	world = new BoxWorld();
	level->genCollision(world);
	level->getEnemyLayer(rc, world, entities, sf::Vector2f(70, 220));
	level->getStarLayer(rc, world, entities);
	level->getStarDustLayer(rc, world, entities);

	sh = createShader("Assets/Shaders/default.vert", "Assets/Shaders/default.frag");
	sh2 = createShader("Assets/Shaders/default.vert", "Assets/Shaders/darkenPass.frag");
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
	level->Render(window);

	for (Entity* e : entities)
	{
		e->render(window);
	}

	//world->drawDebug(window);

	light->clear();
	light2->clear();

	sf::RenderTexture* tx = light->getOccluder();
	sf::View view = tx->getView();
	view.setCenter(light->getPosition());
	tx->setView(view);

	sf::RenderTexture* tx2 = light2->getOccluder();
	view = tx2->getView();
	view.setCenter(light2->getPosition());
	tx2->setView(view);
	for (Entity* e : entities)
	{
		e->render(*tx);
		e->render(*tx2);
	}

	light->calculateShadow();
	light2->calculateShadow();

	fixcolorlight();

	RenderStates states = RenderStates(BlendMode::BlendAdd);
	states.shader = sh;

	darkness1->clear(sf::Color(0, 0, 0, 0));
	darkness1->draw(*light, states);
	darkness1->draw(*light2, states);
	darkness1->display();
	{
		RectangleShape shape = RectangleShape((Vector2f)darkness1->getSize());
		shape.setTexture(&darkness1->getTexture(), false);
		shape.setFillColor(light->getColor());
		sh2->setParameter("darknessColor", sf::Color(0, 0, 10, 180));
		sh2->setParameter("colorTexture", darkness2->getTexture());

		states.blendMode = BlendMode::BlendNone;
		states.shader = sh2;

		////darkness1->clear(Color::Transparent);
		darkness1->draw(shape, states);
		darkness1->display();

		//window.draw(*light);
		Sprite spr = Sprite(darkness1->getTexture());

		states.blendMode = BlendAlpha;
		states.shader = nullptr;
		window.draw(spr, states);
	}
	/*sf::Sprite spriteLol = sf::Sprite(light->getDebugMap());
	sf::RectangleShape shape2 = sf::RectangleShape(sf::Vector2f(512, 512));
	shape2.setFillColor(sf::Color::White);
	window.draw(shape2);
	window.draw(spriteLol);*/

	sf::RectangleShape shape = sf::RectangleShape(light->getSize());
	shape.setOrigin(light->getSize().x / 2.0f, light->getSize().y / 2.0f);
	shape.setPosition(light->getPosition());
	shape.setOutlineColor(sf::Color::Red);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);

	//window.draw(shape);

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