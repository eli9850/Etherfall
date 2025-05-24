#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <filesystem>
#include <iostream>

namespace Etherfall {

	class Climbable final
	{
	public:
		Climbable(uint32_t climbable_id, sf::Vector2f start, float size);
		void draw(sf::RenderWindow& window);
		bool can_climb(const sf::Vector2f& position) const;
		float get_x_position() const;
	private:
		sf::Vector2f m_start;
		float m_size;
		sf::Vector2f m_allow_climb;
	};
}
