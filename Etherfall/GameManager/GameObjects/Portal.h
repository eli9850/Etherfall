#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <filesystem>
#include <iostream>

namespace Etherfall {

	class Portal final
	{
	public:
		Portal(uint32_t portal_id, const sf::Vector2f& position, uint32_t map_id);
		bool is_enter(const std::optional<sf::Event>& event, const sf::Vector2f& player_portal_position) const;
		void handle_frame(uint64_t dt);
		void draw(sf::RenderWindow& window);
		uint32_t get_map_id() const;
		sf::Vector2f get_enter_position() const;
	private:
		std::unique_ptr<sf::Sprite> m_portal_sprite;
		sf::Vector2i m_frame_size;
		int m_number_of_frames;
		int m_number_of_rows;
		int m_current_frame;
		int m_frame_duration;
		uint64_t m_timer;
		uint32_t m_map_id;
		sf::Vector2f m_enter_position;
		sf::Vector2f m_allow_enter;
	};
}
