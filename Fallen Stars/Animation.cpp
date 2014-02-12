////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 Maximilian Wagenbach (aka. Foaly) (foaly.f@web.de)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// This is an altered source version
////////////////////////////////////////////////////////////

#include "Animation.h"

#include <iostream>

Animation::Animation(sf::IntRect rect, const sf::Texture& texture) : 
	m_frames(1),
	m_texture(&texture)
{
	m_frames[0] = rect;
}

Animation::Animation(std::vector<sf::IntRect> frames, const sf::Texture& texture) : 
	m_frames(frames),
	m_texture(&texture)
{
	if(m_frames.empty())
	{
		std::cout << "Animation has no frames!";
	}
}

const sf::Texture* Animation::getSpriteSheet() const
{
    return m_texture;
}

int Animation::getSize() const
{
    return m_frames.size();
}

const sf::IntRect& Animation::getFrame(int n) const
{
	if(m_frames.empty())
	{
		static sf::IntRect missingFrame;
		return missingFrame;
	}
    return m_frames[n];
}