#include "ShaderTestState.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include "LightSource.h"
#include "LevelManager.h"
#include "BoxWorld.h"
#include "Entity.h"
#include "ResourceCollection.h"
#include "BaseResolution.h"
#include "LightSolver.h"

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

	LightSource *light, *light2;

	LightSolver* solver;
}

ShaderTestState::ShaderTestState()
{
	Texture* mask = new Texture();
	mask->loadFromFile("Assets/Shaders/mask.png");

	solver = new LightSolver();

	light = solver->createLight(1024, 1024);
	light2 = solver->createLight(512, 512);

	light->setColor(Color(255, 255, 0, 40));
	light->setMask(mask);
	light2->setPosition(sf::Vector2f(820, 200));
	light2->setColor(Color(255, 0, 0, 50));

	level = new LevelManager("Test");

	world = new BoxWorld();
	level->genCollision(world);
	level->getEnemyLayer(rc, world, entities, sf::Vector2f(70, 220));
	level->getStarLayer(rc, world, entities);
	level->getStarDustLayer(rc, world, entities);

	for (Entity* e : entities)
	{
		solver->addOccluder(e);
	}
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

	solver->render();
	window.draw(sf::Sprite(solver->getResult()));

	//world->drawDebug(window);

	sf::RectangleShape shape = sf::RectangleShape(light->getSize());
	shape.setOrigin(light->getSize().x / 2.0f, light->getSize().y / 2.0f);
	shape.setPosition(light->getPosition());
	shape.setOutlineColor(sf::Color::Red);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(1.0f);

	window.draw(shape);
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