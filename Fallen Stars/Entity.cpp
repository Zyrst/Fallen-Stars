#include "Entity.h"
#include "BoxWorld.h"
#include "Box2D/Box2D.h"
#include "VecConverter.h"

Entity::Entity(BoxWorld* world, sf::Vector2f& size, sf::Vector2f& pos, Facing facing, bool createCollisionBody) :
	Occluder(),
	mAlive(true),
	body(world->createEntityBody(pos, size, createCollisionBody)),
	anime(),
	bodyBounds(pos, size),
	currentFacing(facing)
{
	body->SetUserData(this);
}
Entity::~Entity() 
{
	BoxWorld::destroyBody(body);
}
bool Entity::isAlive()
{
	return mAlive;
}

sf::Vector2f Entity::getPosition()
{
	return Convert::b2ToSfml(body->GetPosition());
}
void Entity::setPosition(float x, float y)
{
	body->SetTransform(Convert::sfmlToB2(sf::Vector2f(x,y)),0);
}

Entity::Facing Entity::getFacing()
{
	return currentFacing;
}

Entity::EntityCategory Entity::getCategory()
{
	return currentCategory;
}

void Entity::setFacing(Facing facing)
{
	if(facing != currentFacing) 
	{
		anime.scale(-1, 1);
	}
	currentFacing = facing;
}

void Entity::render(sf::RenderTarget& target)
{
	if (isAlive())
	{
		anime.setPosition(Convert::b2ToSfml(body->GetPosition()));
		target.draw(anime);
	}
	
}

void Entity::updateSpriteOrigin(SpriteOrigin hor, SpriteOrigin vert)
{
	const sf::FloatRect& psize = anime.getLocalBounds();
	sf::FloatRect fixSize = BoxBounds::boundsOfFixture(body->GetFixtureList());

	float x, y;

	switch (hor)
	{
	case SpriteOrigin::ORIGIN_CENTER:
		x = (psize.width) / 2.0f;
		break;
	case SpriteOrigin::ORIGIN_RIGHT:
		x = psize.width - fixSize.width;
		break;
	case SpriteOrigin::ORIGIN_LEFT:
	default:
		x = 0.0f;
	}

	switch (vert)
	{
	case SpriteOrigin::ORIGIN_CENTER:
		y = (psize.height) / 2.0f - fixSize.height / 2.0f;
		break;
	case SpriteOrigin::ORIGIN_DOWN:
		y = psize.height - fixSize.height;
		break;
	case SpriteOrigin::ORIGIN_UP:
	default:
		y = 0.0f;
	}

	anime.setOrigin(x, y);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(anime, states);
}