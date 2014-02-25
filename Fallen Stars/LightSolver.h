#pragma once

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include "LightSource.h"
#include <vector>
#include "Occluder.h"

namespace tmx
{
	class MapObject;
}

class LightSolver
{
public:
	typedef std::vector<const Occluder*> OccluderVector;

	LightSolver();
	~LightSolver();

	//Creators
	LightSource* createLight(int width, int height, int filterGroup = 255);

	//Removers/destroyers
	void destroyLight(LightSource* light);
	void removeOccluder(const Occluder* occluder);

	//Setters
	void setVoidColor(const sf::Color& color);
	void addOccluder(const Occluder* occluder);
	void addCollisionOccluders(const std::vector<tmx::MapObject>& objects);

	//Getters
	const sf::Color& getVoidColor() const;

	//Renderfunctions
	void render(sf::RenderTarget& target);
	void debugRenderOccluders(sf::RenderTarget& target) const;
	const sf::Texture& getResult() const;

private:
	void pass1();
	void pass2();

	void drawToColorFBO(const LightSource* light);

	std::vector<LightSource*> lights;
	sf::Color voidColor;
	sf::Shader renderShader, debugShader;
	sf::RenderTexture fullScreenBuffer, colorBuffer;

	//All occluders
	OccluderVector occluders;

	//Vector holding every occluder this lightsolver manages memory for.
	std::vector<Occluder*> disposeableOccluders;
};

