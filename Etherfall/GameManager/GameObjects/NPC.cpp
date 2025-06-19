#include "NPC.h"
#include <nlohmann/json.hpp>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	NPC::NPC(uint32_t npc_id, const sf::Vector2f& position)	{

		const auto& npc_details = g_resource_manager->get_npc_details_by_id(npc_id);
		const auto& npc_texture = g_resource_manager->get_npc_texture(npc_details.at("NPC_ID"));
		m_npc_sprite = std::make_unique<sf::Sprite>(npc_texture);
		m_image = npc_texture.copyToImage();
		m_npc_sprite->setPosition(position);
		m_current_frame = 0;
		m_timer = 0;
		m_frame_duration = npc_details.at("FrameDuration");
		m_number_of_rows = npc_details.at("Rows");
		m_number_of_frames = npc_details.at("FrameNumber");
		m_frame_size = { npc_details.at("FrameSize")[0], npc_details.at("FrameSize")[1] };
		//m_npc_sprite->setTextureRect({ {0, 0}, m_frame_size });
		/*sf::Vector2f scale{npc_details.at("Size")[0] / static_cast<float>(m_npc_sprite->getTextureRect().size.x),
			npc_details.at("Size")[1] / static_cast<float>(m_npc_sprite->getTextureRect().size.y)};
		m_npc_sprite->setScale(scale);*/
	}

	bool NPC::is_clicked(const sf::RenderWindow& window, const sf::Vector2i& mouse_position) const {
		
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mouse_position);
		if (m_npc_sprite->getGlobalBounds().contains(mouseWorld)) {
			sf::Vector2f local = mouseWorld - m_npc_sprite->getPosition();
			uint32_t x = static_cast<uint32_t>(local.x);
			uint32_t y = static_cast<uint32_t>(local.y);
			if (x >= 0 && x < (int)m_image.getSize().x &&
				y >= 0 && y < (int)m_image.getSize().y) {
				sf::Color pixel = m_image.getPixel({ x, y });
				if (pixel.a > 0) {
					return true;
				}
			}
		}
		return false;
	}

	void NPC::handle_frame(uint64_t dt) {
		m_timer += dt;
		if (m_timer > m_frame_duration) {
			m_timer %= m_frame_duration;
			m_current_frame++;
			m_current_frame %= m_number_of_frames;
			m_npc_sprite->setTextureRect(sf::IntRect{ {m_frame_size.x* (m_current_frame % m_number_of_rows),
				m_frame_size.y* (m_current_frame / m_number_of_rows)},
				{ m_frame_size.x, m_frame_size.y } });
		}
	}

	void NPC::draw(sf::RenderWindow& window) {
		window.draw(*m_npc_sprite);
	}
}