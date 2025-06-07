#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

namespace Etherfall {

	class Wall final
	{
	public:
		Wall(sf::Vector2f start, float size);
		void draw(sf::RenderWindow& window);
		bool is_through_wall(const sf::Vector2f& pos, float player_size, float distance) const;
		float get_x() const;
	private:
		sf::Vector2f m_start;
		float m_size;
	};
}
