#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>

namespace Etherfall {

	struct ParallaxDetails {
		float factor;
		std::unique_ptr<sf::Sprite> image;	
	};

	class Parallax
	{
	public:
		Parallax(const nlohmann::json& details, const sf::Vector2u& map_size);
		void update(float x);
		void draw(sf::RenderWindow& window);
	private:
		std::vector<ParallaxDetails> m_parallaxs;
	};
}
