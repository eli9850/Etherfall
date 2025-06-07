#include "Wall.h"
#include <json.hpp>

namespace Etherfall {

	Wall::Wall(sf::Vector2f start, float size)
		: m_start(std::move(start)), m_size(size) {}

	bool Wall::is_through_wall(const sf::Vector2f& pos, float player_size, float distance) const {
		if (pos.y >= m_start.y && pos.y <= m_start.y + m_size) {
			if (distance < 0) {
				if (pos.x - player_size >= m_start.x && pos.x - player_size + distance <= m_start.x) {
					return true;
				}
			}
			else {
				if (pos.x + player_size <= m_start.x && pos.x +player_size + distance >= m_start.x) {
					return true;
				}
			}
		}
		return false;
	}

	float Wall::get_x() const {
		return m_start.x;
	}

	void Wall::draw(sf::RenderWindow& window) {
		sf::Vertex line[] = {
		   sf::Vertex(m_start, sf::Color::White),
		   sf::Vertex({m_start.x, m_start.y + m_size}, sf::Color::Green)
		};
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}