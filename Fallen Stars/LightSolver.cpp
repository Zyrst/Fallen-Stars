#include "LightSolver.h"
#include "BaseResolution.h"
#include <iostream>
#include <SFML\Graphics\RectangleShape.hpp>

LightSolver::LightSolver()
: lights()
, voidColor(sf::Color(0, 0, 10, 180))
, renderShader()
, fullScreenBuffer()
, colorBuffer()
{
	renderShader.loadFromFile("Assets/Shaders/default.vert", "Assets/Shaders/darkenPass.frag");
	fullScreenBuffer.create(baseWidth, baseHeight);
	colorBuffer.create(baseWidth, baseHeight);
}


LightSolver::~LightSolver()
{
	for (LightSource* src : lights)
	{
		delete src;
	}
}

LightSource* LightSolver::createLight(int width, int height)
{
	LightSource* light = new LightSource(width, height);
	lights.push_back(light);
	return light;
}

void LightSolver::destroyLight(LightSource* light)
{
	for (auto i = lights.begin(); i != lights.end(); i++)
	{
		if (*i == light)
		{
			std::cout << "Found light: " << light << "\n";
			lights.erase(i);
			delete light;
			return;
		}
	}

	std::cout << "Light was not found: " << light << "\n";
}

void LightSolver::removeOccluder(const sf::Drawable* occluder)
{
	for (auto i = occluders.begin(); i != occluders.end(); i++)
	{
		if (*i == occluder)
		{
			std::cout << "Found occluder: " << occluder << "\n";
			occluders.erase(i);
			return;
		}
	}

	std::cout << "Occluder was not found: " << occluder << "\n";
}

//Setters
void LightSolver::setVoidColor(const sf::Color& color)
{
	voidColor = color;
}

void LightSolver::addOccluder(const sf::Drawable* occluder)
{
	occluders.push_back(occluder);
}

//Getters
const sf::Color& LightSolver::getVoidColor() const
{
	return voidColor;
}

//Renderfunctions
void LightSolver::render()
{
	fullScreenBuffer.clear(sf::Color::Transparent);
	colorBuffer.clear(sf::Color::Transparent);

	//First pass calculates the lights based on the occluders
	//and writes their colors to the color buffer
	pass1();

	//Second pass merges the color buffer with the fullscreen buffer.
	pass2();
}

const sf::Texture& LightSolver::getResult() const
{
	return fullScreenBuffer.getTexture();
}

void LightSolver::pass1()
{
	//Clear the lights and draw occluders into them.
	for (LightSource* light : lights)
	{
		light->clear();
		sf::RenderTexture* tx = light->getOccluder();

		//Set up the view of the light.
		sf::View view = tx->getView();
		view.setCenter(light->getPosition());
		tx->setView(view);

		//Draw occluders
		for (const sf::Drawable* occluder : occluders)
		{
			tx->draw(*occluder);
		}

		//Calculate shadows
		light->calculateShadow();

		//Draw the light into the color buffer.
		drawToColorFBO(light);

		fullScreenBuffer.draw(*light, sf::BlendMode::BlendAdd);
	}

	//Once all lights are rendered, update the respective fbo-textures.
	fullScreenBuffer.display();
	colorBuffer.display();
}

void LightSolver::pass2()
{
	sf::RectangleShape rect = sf::RectangleShape((sf::Vector2f) fullScreenBuffer.getSize());
	rect.setTexture(&fullScreenBuffer.getTexture());

	//Set the parameters used by the shaders.
	renderShader.setParameter("voidColor", voidColor);
	renderShader.setParameter("colorTexture", colorBuffer.getTexture());

	//We want to overwrite the entire buffer when we merge
	sf::RenderStates states = sf::RenderStates(sf::BlendMode::BlendNone);
	states.shader = &renderShader;

	//DO MAGIC PLS
	fullScreenBuffer.draw(rect, states);
	fullScreenBuffer.display();
}

void LightSolver::drawToColorFBO(const LightSource* light)
{
	sf::RenderStates states = sf::RenderStates(sf::BlendMode::BlendAdd);

	//Set up and render a rectangle representing the color of the light.
	sf::RectangleShape rect = sf::RectangleShape(light->getSize());
	rect.setOrigin(light->getSize().x / 2.0f, light->getSize().y / 2.0f);
	rect.setFillColor(light->getColor());
	rect.setPosition(light->getPosition());
	rect.setTexture(&light->getResult());
	colorBuffer.draw(rect, states);
}