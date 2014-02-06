#include "JumpingTest.h"
#include "BoxWorld.h"
#include <Box2D\Box2D.h>
#include "LevelManager.h"
#include "VecConverter.h"
#include "CallBack.h"
#include "Player.h"

namespace
{
	bool jump = false, up = false, down = false, left = false, right = false;
	LevelManager* level;
	BoxWorld* world;
	Player* player;

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
	level = new LevelManager("test");

	auto pos = sf::Vector2f(64, 32);
	auto size = sf::Vector2f(20, 32);
	genCollision();
	player = new Player(sf::Sprite(), world, size, pos);

	//setupGroundSensor(player, pos, size);

	//player->SetFixedRotation(false);
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
}

void JumpingTest::handleAction(Controls::Action action, Controls::KeyState state)
{
	/*bool hej = (state == Controls::KeyState::HELD);

	switch (action)
	{
	case Controls::Action::DOWN:
		down = hej;
		break;
	case Controls::Action::UP:
		up = hej;
		break;
	case Controls::Action::LEFT:
		left = hej;
		break;
	case Controls::Action::RIGHT:
		right = hej;
		break;
	case Controls::Action::JUMP:
		jump = hej;
		break;
	}*/

	player->handleAction(action, state);
}
