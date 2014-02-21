#pragma once

#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Shader.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include "LightSource.h"
#include <vector>

class LightSolver
{
public:
	typedef std::vector<const sf::Drawable*> OccluderVector;

	LightSolver();
	~LightSolver();

	//Creators
	LightSource* createLight(int width, int height);

	//Removers/destroyers
	void destroyLight(LightSource* light);
	void removeOccluder(const sf::Drawable* occluder);

	//Setters
	void setVoidColor(const sf::Color& color);
	void addOccluder(const sf::Drawable* occluder);

	//Getters
	const sf::Color& getVoidColor() const;

	//Renderfunctions
	void render(sf::RenderTarget& target);
	const sf::Texture& getResult() const;

private:
	void pass1();
	void pass2();

	void drawToColorFBO(const LightSource* light);

	std::vector<LightSource*> lights;
	sf::Color voidColor;
	sf::Shader renderShader;
	sf::RenderTexture fullScreenBuffer, colorBuffer;
	OccluderVector occluders;
};

