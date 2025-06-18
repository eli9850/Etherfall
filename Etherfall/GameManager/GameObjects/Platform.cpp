#include "Platform.h"

namespace Etherfall {

	Platform::Platform(std::vector<platformEquation> platform) : m_equations(std::move(platform)) {
		m_range.x = m_equations[0].range.x;
		m_range.y = m_equations[m_equations.size() - 1].range.y;
	}

	float Platform::getYAtX(float x) const {
		const auto& equation = get_equation(x);
		if (x < m_range.x) {
			x = m_range.x;
		}
		else if (x > m_range.y) {
			x = m_range.y;
		}
		return equation.a0 * x + equation.a1;
	}

	bool Platform::isWithinX(float x) const {
		return x >= m_range.x && x <= m_range.y;
	}

	const platformEquation& Platform::get_equation(float x) const {
		if (x <= m_range.x) {
			return m_equations[0];
		}
		if (x >= m_range.y) {
			return m_equations[m_equations.size() - 1];
		}

		for (const auto& equation : m_equations) {
			if (x >= equation.range.x && x <= equation.range.y) {
				return equation;
			}
		}
		throw std::runtime_error("invalid equation");
	}

	float Platform::get_slope(float x, float distance) const {
		auto y = getYAtX(x);
		auto x2 = x + distance;
		auto y2 = getYAtX(x2);
		auto slope = (y - y2) / (x - x2);
		return slope;
	}

	void Platform::draw(sf::RenderWindow& window) {

		std::vector<sf::VertexArray> platforms;

		for (const auto& e : m_equations) {
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			float x1 = e.range.x;
			float y1 = getYAtX(x1);
			float x2 = e.range.y;
			float y2 = getYAtX(x2);
			line[0].position = { x1, y1 };
			line[1].position = { x2, y2 };
			line[0].color = line[1].color = sf::Color::White;
			platforms.push_back(line);
		}
		for (const auto& platform : platforms)
			window.draw(platform);

	}
}