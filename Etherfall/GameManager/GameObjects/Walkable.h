#pragma once

#include <SFML/Graphics.hpp>

namespace Etherfall {

	class Walkable
	{
	public:
		virtual void draw(sf::RenderWindow& window) = 0;
		virtual float getYAtX(float x) const = 0;
		virtual bool isWithinX(float x) const = 0;
		sf::Vector2f get_next_position(float x, float distance) {
			auto slope = get_slope(x, distance);
			float dx = 0;
			auto length = static_cast<float>(std::pow(1 + slope * slope, 0.5));
			dx = distance / length;
			return { x + dx, getYAtX(x + dx)};
		}
		virtual float get_slope(float x, float distance) const = 0;
	};
}
