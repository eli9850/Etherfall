#include "Parallax.h"
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	Parallax::Parallax(const nlohmann::json& details, const sf::Vector2u& map_size) {
		for (const auto& parallax : details) {
			ParallaxDetails parallax_details;
			parallax_details.factor = parallax.at("Factor");
			auto& texture = g_resource_manager->get_parallax_texture(parallax.at("Image"));
			bool is_repeated = parallax.at("Repeated");
			texture.setRepeated(is_repeated);
			parallax_details.image = std::make_unique<sf::Sprite>(texture);
			sf::Vector2f pos = { parallax.at("Pos")[0], parallax.at("Pos")[1] };
			parallax_details.image->setTextureRect({ {0, 0}, {(int)map_size.x, (int)texture.getSize().y}});
			if (is_repeated) {
				parallax_details.image->setPosition({0, pos.y});
			}
			else {
				parallax_details.image->setPosition(pos);
			}
			m_parallaxs.push_back(std::move(parallax_details));
		}
	}

	void Parallax::update(float x) {
		for (const auto& parallax : m_parallaxs) {
			parallax.image->move({ x * parallax.factor, 0 });
		}
	}

	void Parallax::draw(sf::RenderWindow& window) {
		for (const auto& parallax : m_parallaxs) {
			window.draw(*parallax.image);
		}
	}
}