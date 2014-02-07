#include "JumpingTest.h"
#include "BoxWorld.h"
#include <Box2D\Box2D.h>
#include "LevelManager.h"
#include "VecConverter.h"
#include "CallBack.h"
#include "Player.h"
#include "ResourceCollection.h"

namespace
{
	bool jump = false, up = false, down = false, left = false, right = false;
	LevelManager* level;
	BoxWorld* world;
	Player* player;

	ResourceCollection rc;
	sf::Texture* tx;
	sf::Sprite* sp;

	void genCollision()
	{
		auto& layers = level->getMapLoader().GetLayers();

		for (auto& l : layers)
		{
			if (l.name.compare("collision") == 0)
			{
				world->createStaticBody(l.objects);
				break;
			}
		}
	}
}

JumpingTest::JumpingTest()
{ 
	world = new BoxWorld(b2Vec2(0, 30));
	level = new LevelManager("proto");

	auto pos = sf::Vector2f(98, 32);
	auto size = sf::Vector2f(100, 32);
	genCollision();

	tx = new sf::Texture();
	tx->loadFromFile("Assets/Map/Stella pixel.png");

	sp = new sf::Sprite(*tx);

	player = new Player(*sp, world, size, pos);
}

JumpingTest::~JumpingTest()
{ 
	delete world;
	delete level;
	delete player;
}

void JumpingTest::update(sf::Time deltaTime)
{
	player->update(deltaTime);

	world->step(deltaTime.asSeconds());
}
void JumpingTest::render(sf::RenderWindow& window)
{
	level->Render(window);
	world->drawDebug(window);
	player->render(window);
}

void JumpingTest::handleAction(Controls::Action action, Controls::KeyState state)
{
	player->handleAction(action, state);
}
