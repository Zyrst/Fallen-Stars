#include "JumpingTest.h"
#include "BoxWorld.h"
#include <Box2D\Box2D.h>
#include "LevelManager.h"
#include "VecConverter.h"
#include "CallBack.h"
#include "Player.h"
#include "ResourceCollection.h"
#include "Camera.h"
#include "Object.h"

namespace
{
	bool jump = false, up = false, down = false, left = false, right = false;
	LevelManager* level;
	BoxWorld* world;
	Player* player;
	Camera* camera;
	Object* object;

	//ResourceCollection rc;
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

	level = new LevelManager("Test");
	
//	mResourceCollection.preloadTexture("Assets/Map/Stella_idle.png");
//	mResourceCollection.preloadTexture("Assets/Map/Stella Left.png");
//	mResourceCollection.preloadTexture("Assets/Map/Stella_jumpLeft.png");
//	mResourceCollection.preloadTexture("Assets/Map/Stella_grabLeft.png");
//	mResourceCollection.preloadTexture("Assets/Map/trashcanfull.png");
//	mResourceCollection.preloadTexture("Assets/Map/Window_06.png");

	auto size = sf::Vector2f(70, 220);
	genCollision();


	auto& layers = level->getMapLoader().GetLayers();
	for(auto i : layers)
	{
		if (i.name.compare("Player") == 0)
		{
			auto j = i.objects;
			for ( auto k : j)
			{
				auto pos = k.GetPosition();
				player = new Player(world, size, pos,mResourceCollection);
			}
		}
		if (i.name.compare("Star") == 0)
		{
			auto j = i.objects;
			for (auto k : j)
			{
				auto pos = k.GetPosition();
				object = new Object(world,pos,mResourceCollection, Object::TYPE::STAR);
			}
		}
	}

	object = new Object(world, sf::Vector2f(500, 100), mResourceCollection, Object::TYPE::STAR);

	sf::Vector2u mapSize =  level->getMapLoader().GetMapSize();
	camera = new Camera(player, mapSize);
}

JumpingTest::~JumpingTest()
{ 
	delete world;
	delete level;
	delete player;
}

void JumpingTest::update(const sf::Time& deltaTime)
{
	player->update(deltaTime);
	object->update(deltaTime);

	world->step(deltaTime.asSeconds());
}
void JumpingTest::render(sf::RenderWindow& window)
{
	camera->update(window);
	level->getMapLoader().Draw(window, tmx::MapLayer::Background);
	player->render(window);
	object->render(window);
	level->getMapLoader().Draw(window, tmx::MapLayer::Foreground);
	world->drawDebug(window);
}

void JumpingTest::handleAction(Controls::Action action, Controls::KeyState state)
{
	player->handleAction(action, state);
}
