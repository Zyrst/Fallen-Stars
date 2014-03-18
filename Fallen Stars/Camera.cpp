#include "Camera.h"
#include <SFML/Graphics/Rect.hpp>
#include "Player.h"



Camera::Camera(Player* player, sf::Vector2u bounds):
	mPlayer(player),
	mBounds(bounds)
{}

Camera::~Camera()
{}

void Camera::update(sf::RenderTarget& target)
{
	sf::View view = target.getView(); 
	sf::Vector2f new_view_center = mPlayer->getPosition();
	new_view_center.x += 500;
	sf::Vector2f half_view_size = view.getSize() * 0.5f;
	if(new_view_center.x - half_view_size.x < 0)
	{
		new_view_center.x = half_view_size.x;
	}
	else if(new_view_center.x + half_view_size.x > mBounds.x)
	{
		new_view_center.x = mBounds.x - half_view_size.x;
	}
	
	if(new_view_center.y - half_view_size.y < 0)
	{
		new_view_center.y = half_view_size.y;
	}
	else if(new_view_center.y + half_view_size.y > mBounds.y)
	{
		new_view_center.y = mBounds.y - half_view_size.y;
	}

	view.setCenter(new_view_center);
	target.setView(view);
}