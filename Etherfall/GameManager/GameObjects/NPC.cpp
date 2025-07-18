#include "NPC.h"
#include <nlohmann/json.hpp>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	NPC::NPC(uint32_t npc_id, const sf::Vector2f& position) : m_id(npc_id), m_label(g_resource_manager->get_font()){

		const auto& npc_details = g_resource_manager->get_npc_details_by_id(m_id);
		const auto& npc_texture = g_resource_manager->get_npc_texture(m_id);
		m_name = npc_details.at("Name");
		for (const auto& text : npc_details.at("Text")) {
			m_text.push_back(text);
		}
		m_npc_sprite = std::make_unique<sf::Sprite>(npc_texture);
		m_image = npc_texture.copyToImage();
		m_npc_sprite->setPosition(position);
		m_current_frame = 0;
		m_timer = 0;
		m_frame_duration = npc_details.at("FrameDuration");
		m_number_of_rows = npc_details.at("Rows");
		m_number_of_frames = npc_details.at("FrameNumber");
		m_frame_size = { npc_details.at("FrameSize")[0], npc_details.at("FrameSize")[1] };
		m_npc_sprite->setTextureRect({ {0, 0}, m_frame_size });
		m_scale = {npc_details.at("Size")[0] / static_cast<float>(m_npc_sprite->getTextureRect().size.x),
			npc_details.at("Size")[1] / static_cast<float>(m_npc_sprite->getTextureRect().size.y)};
		m_npc_sprite->setScale(m_scale);
		initialize_label();
	}

	void NPC::initialize_label() {

		auto label_x_size = static_cast<float>(m_name.size() * 10);
		sf::Vector2f label_position = { m_npc_sprite->getPosition().x + m_scale.x * m_frame_size.x / 2 - label_x_size / 2 , m_npc_sprite->getPosition().y + m_scale.y * m_frame_size.y};
		m_shape.setSize({ label_x_size, 20 });
		m_shape.setFillColor(sf::Color(50, 50, 50, 150));
		m_shape.setOutlineThickness(1);
		m_shape.setOutlineColor(sf::Color::White);
		m_shape.setPosition(label_position);

		m_label.setString(m_name);
		m_label.setCharacterSize(14);
		m_label.setFillColor(sf::Color::White);
		m_label.setPosition({ label_position.x + 10, label_position.y + 5 });
	}

	bool NPC::is_clicked(const sf::RenderWindow& window, const sf::Vector2i& mouse_position, const sf::View& view) const {
		
		sf::Vector2f mouseWorld = window.mapPixelToCoords(mouse_position, view);
		if (m_npc_sprite->getGlobalBounds().contains(mouseWorld)) {
			sf::Vector2f local = (mouseWorld - m_npc_sprite->getPosition());
			local = { local.x / m_scale.x, local.y / m_scale.y };
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
		window.draw(m_shape);
		window.draw(m_label);
		window.draw(*m_npc_sprite);
	}

	const std::vector<std::string>& NPC::get_dialogue() const {
		return m_text;
	}

	uint32_t NPC::get_id() const {
		return m_id;
	}

	const std::string& NPC::get_name() const {
		return m_name;
	}
}