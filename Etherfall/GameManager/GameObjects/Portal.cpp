#include "Portal.h"
#include <nlohmann/json.hpp>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	Portal::Portal(uint32_t portal_id, const sf::Vector2f& position, uint32_t map_id)
		: m_map_id(map_id)
	{
		const auto& portal_details = g_resource_manager->get_portal_details_by_id(portal_id);
		const auto& portal_texture = g_resource_manager->get_portal_texture(portal_details.at("PortalID"));
		m_portal_sprite = std::make_unique<sf::Sprite>(portal_texture);
		m_portal_sprite->setPosition(position);
		m_current_frame = 0;
		m_timer = 0;
		m_frame_duration = portal_details.at("FrameDuration");
		m_number_of_rows = portal_details.at("Rows");
		m_number_of_frames = portal_details.at("FrameNumber");
		m_frame_size = { portal_details.at("FrameSize")[0], portal_details.at("FrameSize")[1] };
		m_portal_sprite->setTextureRect({ {0, 0}, m_frame_size });
		sf::Vector2f scale{portal_details.at("Size")[0] / (float)m_portal_sprite->getTextureRect().size.x,
			portal_details.at("Size")[1] / (float)m_portal_sprite->getTextureRect().size.y};
		m_portal_sprite->setScale(scale);

		m_enter_position.x = position.x + static_cast<float>(portal_details.at("EnterPosition")[0]) * portal_details.at("Size")[0];
		m_enter_position.y = position.y + static_cast<float>(portal_details.at("EnterPosition")[1]) * portal_details.at("Size")[1];
		m_allow_enter = { static_cast<float>(portal_details.at("AllowEnter")[0]) * portal_details.at("Size")[0],
						  static_cast<float>(portal_details.at("AllowEnter")[1]) * portal_details.at("Size")[1] };
	}

	bool Portal::is_enter(const std::optional<sf::Event>& event, const sf::Vector2f& player_portal_position) const {

		if (!event) {
			return false;
		}

		const auto* const key_event = event->getIf<sf::Event::KeyPressed>();
		if (!key_event || key_event->code != sf::Keyboard::Key::Up) {
			return false;
		}

		if (std::abs(player_portal_position.x - m_enter_position.x) < m_allow_enter.x &&
			std::abs(player_portal_position.y - m_enter_position.y) < m_allow_enter.y) {
			return true;
		}
		return false;
	}

	void Portal::handle_frame(uint64_t dt) {
		m_timer += dt;
		if (m_timer > m_frame_duration) {
			m_timer %= m_frame_duration;
			m_current_frame++;
			m_current_frame %= m_number_of_frames;
			m_portal_sprite->setTextureRect(sf::IntRect{ {m_frame_size.x * (m_current_frame % m_number_of_rows), 
																   m_frame_size.y * (m_current_frame / m_number_of_rows)},
																  { m_frame_size.x, m_frame_size.y } });
		}
	}

	void Portal::draw(sf::RenderWindow& window) {
		window.draw(*m_portal_sprite);
	}

	uint32_t Portal::get_map_id() const {
		return m_map_id;
	}

	sf::Vector2f Portal::get_enter_position() const {
		return m_enter_position;
	}
}