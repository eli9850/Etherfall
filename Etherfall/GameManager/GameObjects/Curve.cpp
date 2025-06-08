#include "Curve.h"
#include "SFML/Graphics/PrimitiveType.hpp"

namespace Etherfall {

	Curve::Curve(std::vector<equation> curve): m_equations(std::move(curve)) {
		m_range.x = m_equations[0].range.x;
		m_range.y = m_equations[m_equations.size() - 1].range.y;
	}

	float Curve::getYAtX(float x) const {
		const auto& equation = get_equation(x);
		auto x2 = x * x;
		auto x3 = x2 * x;
		return equation.a0 + equation.a1 * x + equation.a2 * x2 + equation.a3 * x3;
	}

	bool Curve::isWithinX(float x) const {
		return x >= m_range.x && x <= m_range.y;
	}

	void Curve::draw(sf::RenderWindow& window) {
		
		std::vector<sf::VertexArray> curves;
		for (const auto& e : m_equations) {
			int samples = 10;
			sf::VertexArray curve(sf::PrimitiveType::LineStrip, samples);

			float step = (m_range.y - m_range.x) / (samples - 1);
			for (int i = 0; i < samples; ++i) {
				float x = m_range.x + i * step;
				float y = getYAtX(x);
				curve[i] = sf::Vertex(sf::Vector2f(x, y), sf::Color::Cyan);
			}
			curves.push_back(curve);
		}
		for (const auto& curve : curves)
			window.draw(curve);
	}

	const equation& Curve::get_equation(float x) const {
		
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
}