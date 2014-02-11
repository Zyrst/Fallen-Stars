#include "JumpingTest.h"
#include "BoxWorld.h"
#include <Box2D\Box2D.h>
#include "LevelManager.h"
#include "VecConverter.h"
#include "CallBack.h"
#include "Player.h"
#include "ResourceCollection.h"
#include "Camera.h"

namespace
{
	bool jump = false, up = false, down = false, left = false, right = false;
	LevelManager* level;
	BoxWorld* world;
	Player* player;
	Camera* camera;

	ResourceCollection rc;
	sf::Texture* tx;
	sf::Sprite* sp;

	void genCollision()
	{
		auto& layers = level->getMapLoader().GetLayers();

		for (auto& l : layers)
		{
			if (l.name.compare("Collision") == 0)
			{
				world->createStaticBody(l.objects);
				break;
			}
		}
	}
}

JumpingTest::JumpingTest()
{ 
	world = new BoxWorld(b2Vec2(0, 10));
	level = new LevelManager("hejsan");

	auto pos = sf::Vector2f(98, 380);
	auto size = sf::Vector2f(70, 220);
	genCollision();

	tx = new sf::Texture();
	tx->loadFromFile("Assets/Map/Stella Pixel.png");

	player = new Player(*tx, world, size, pos);
	sf::Vector2u mapSize =  level->getMapLoader().GetMapSize();
	camera = new Camera(player, mapSize);
}

JumpingTest::~JumpingTest()
{ 
	delete world;
	delete level;
	delete player;
}

void JumpingTest::update(sf::Time& deltaTime)
{
	player->update(deltaTime);

	world->step(deltaTime.asSeconds());
}
void JumpingTest::render(sf::RenderWindow& window)
{
	camera->update(window);
	level->getMapLoader().Draw(window, tmx::MapLayer::Background);
	
	player->render(window);
	level->getMapLoader().Draw(window, tmx::MapLayer::Foreground);
	world->drawDebug(window);
}

void JumpingTest::handleAction(Controls::Action action, Controls::KeyState state)
{
	player->handleAction(action, state);
}
