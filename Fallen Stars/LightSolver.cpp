#include "LightSolver.h"
#include "BaseResolution.h"
#include <tmx\MapObject.h>
#include <iostream>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include "ConvexOccluder.h"

LightSolver::LightSolver()
: lights()
, voidColor(sf::Color(0, 0, 10, 180))
, renderShader()
, debugShader()
, fullScreenBuffer()
, colorBuffer()
{
	renderShader.loadFromFile("Assets/Shader/default.vert", "Assets/Shader/darkenPass.frag");
	debugShader.loadFromFile("Assets/Shader/default.vert", "Assets/Shader/occluder.frag");
	fullScreenBuffer.create(baseWidth, baseHeight);
	colorBuffer.create(baseWidth, baseHeight);
}


LightSolver::~LightSolver()
{
	for (LightSource* src : lights)
	{
		delete src;
	}

	for (sf::Drawable* draw : disposeableOccluders)
	{
		delete draw;
	}
}

LightSource* LightSolver::createLight(int width, int height, int filterGroup)
{
	LightSource* light = new LightSource(width, height, filterGroup);
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

void LightSolver::removeOccluder(const Occluder* occluder)
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

void LightSolver::addOccluder(const Occluder* occluder)
{
	occluders.push_back(occluder);
}

void LightSolver::addCollisionOccluders(const std::vector<tmx::MapObject>& objects)
{
	for (auto& o : objects)
	{
		auto& points = o.PolyPoints();
		int size = points.size();
		ConvexOccluder* shape = new ConvexOccluder(size);

		for (int i = 0; i < size; i++)
		{
			shape->setPoint(i, points[i]);
		}

		shape->setPosition(o.GetPosition());

		occluders.push_back(shape);
		disposeableOccluders.push_back(shape);
	}
}

//Getters
const sf::Color& LightSolver::getVoidColor() const
{
	return voidColor;
}

//Renderfunctions
void LightSolver::render(sf::RenderTarget& target)
{
	fullScreenBuffer.clear(sf::Color::Transparent);
	colorBuffer.clear(sf::Color::Transparent);

	sf::View originalView = fullScreenBuffer.getView();
	const sf::View& view = target.getView();
	fullScreenBuffer.setView(view);
	colorBuffer.setView(view);

	//First pass calculates the lights based on the occluders
	//and writes their colors to the color buffer
	pass1();

	//Restore the original view.
	fullScreenBuffer.setView(originalView);
	colorBuffer.setView(originalView);

	//Second pass merges the color buffer with the fullscreen buffer.
	pass2();

	sf::Sprite sprite = sf::Sprite(getResult());
	sf::Vector2f pos = sf::Vector2f(view.getCenter().x - view.getSize().x / 2.0f, view.getCenter().y - view.getSize().y / 2.0f);
	sprite.setPosition(pos);
	target.draw(sprite);
}

void LightSolver::debugRenderOccluders(sf::RenderTarget& target) const
{
	sf::RenderStates states = sf::RenderStates(&debugShader);
	for (const sf::Drawable* d : occluders)
	{
		target.draw(*d, states);
	}
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
		for (const Occluder* occluder : occluders)
		{
			if ((light->getFilterGroup() & occluder->getFilterGroup()) != 0)
			{
				tx->draw(*occluder);
			}
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