#include "Climbable.h"
#include <json.hpp>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	Climbable::Climbable(uint32_t climbable_id, sf::Vector2f start, float size)
		: m_start(std::move(start)), m_size(size) {
		const auto& climbable_details = g_resource_manager->get_climbable_details_by_id(climbable_id);
		m_allow_climb = { climbable_details.at("AllowClimb")[0], climbable_details.at("AllowClimb")[1] };
	}

	bool Climbable::can_climb_up(const sf::Vector2f& position) const{
		if (std::abs(position.x - m_start.x) < m_allow_climb.x &&
			position.y <= m_start.y + m_size + m_allow_climb.y && position.y > m_start.y - m_allow_climb.y) {
			return true;
		}
		return false;
	}

	bool Climbable::can_climb_down(const sf::Vector2f& position) const {
		if (std::abs(position.x - m_start.x) < m_allow_climb.x &&
			position.y < m_start.y + m_size + m_allow_climb.y && position.y >= m_start.y - m_allow_climb.y) {
			return true;
		}
		return false;
	}

	float Climbable::get_x_position() const {
		return m_start.x;
	}

	void Climbable::draw(sf::RenderWindow& window) {
		sf::Vertex line[] = {
		   sf::Vertex(m_start, sf::Color::White),
		   sf::Vertex({m_start.x, m_start.y + m_size}, sf::Color::White)
		};
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}