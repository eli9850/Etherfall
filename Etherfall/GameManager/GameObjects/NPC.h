#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <filesystem>
#include <iostream>

namespace Etherfall {

	class NPC final
	{
	public:
		NPC(uint32_t npc_id, const sf::Vector2f& position);
		bool is_clicked(const sf::RenderWindow& window, const sf::Vector2i& mouse_position) const;
		void handle_frame(uint64_t dt);
		void draw(sf::RenderWindow& window);
	private:
		std::unique_ptr<sf::Sprite> m_npc_sprite;
		sf::Image m_image;
		sf::Vector2i m_frame_size;
		int m_number_of_frames;
		int m_number_of_rows;
		int m_current_frame;
		int m_frame_duration;
		uint64_t m_timer;
	};
}
