#pragma once

#include <SFML/Graphics.hpp>

namespace Etherfall {

	class Walkable
	{
	public:
		virtual void draw(sf::RenderWindow& window) = 0;
		virtual float getYAtX(float x) const = 0;
		virtual bool isWithinX(float x) const = 0;
	};
}
