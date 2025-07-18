#pragma once

#include <SFML/Graphics/Text.hpp>
#include <string>
#include "GameManager/SFMLObjects/RoundedRectangle.h"

namespace Etherfall {

	class Button
	{
	public:
		Button(std::string text,
			uint32_t text_size, 
			const sf::Vector2f& size,
			const sf::Color& topColor = sf::Color::Black,
			const sf::Color& bottomColor = sf::Color::Black,
			float radius = 10.f,
			unsigned int cornerPoints = 8);
		bool is_hover(const sf::Vector2f& position);
		void draw(sf::RenderWindow& window);
		void set_position(const sf::Vector2f& position);
		void set_text_color(const sf::Color& color);
	private:
		sf::Text m_text;
		SFML::RoundedRectangle m_rect;
		
	};

}

