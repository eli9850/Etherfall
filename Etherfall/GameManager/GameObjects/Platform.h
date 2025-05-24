#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <filesystem>
#include <iostream>

namespace Etherfall {

	class Platform final
	{
	public:
		Platform(sf::Vector2f start, sf::Vector2f end);
		void draw(sf::RenderWindow& window);
		float getYAtX(float x) const;
		bool isWithinX(float x) const;
	private:
		sf::Vector2f m_start;
		sf::Vector2f m_end;
		float m_slope;
		float m_intercept;
	};
}
