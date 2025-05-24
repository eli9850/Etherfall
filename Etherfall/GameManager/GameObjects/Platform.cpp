#include "Platform.h"
#include <json.hpp>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	Platform::Platform(sf::Vector2f start, sf::Vector2f end)
		: m_start(std::move(start)), m_end(std::move(end)) {
		m_slope = (m_end.y - m_start.y) / (m_end.x - m_start.x);
		m_intercept = m_start.y - m_slope * m_start.x;
	}

	float Platform::getYAtX(float x) const {

		return m_slope * x + m_intercept;
	}

	bool Platform::isWithinX(float x) const {
		return (x >= std::min(m_start.x, m_end.x) && x <= std::max(m_start.x, m_end.x));
	}

	void Platform::draw(sf::RenderWindow& window) {
		sf::Vertex line[] = {
		   sf::Vertex(m_start, sf::Color::White),
		   sf::Vertex(m_end, sf::Color::White)
		};
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}