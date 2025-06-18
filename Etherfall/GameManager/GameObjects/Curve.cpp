#include "Curve.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <boost/config.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using boost::multiprecision::cpp_dec_float_100;

namespace Etherfall {

	Curve::Curve(std::vector<curveEquation> curve): m_equations(std::move(curve)) {
		m_range.x = m_equations[0].range.x;
		m_range.y = m_equations[m_equations.size() - 1].range.y;
	}

	float Curve::getYAtX(float x) const {
		const auto& equation = get_equation(x);
		if (x < m_range.x) {
			x = m_range.x;
		} else if (x > m_range.y) {
			x = m_range.y;
		}
		
		cpp_dec_float_100 x1_100 = x;
		cpp_dec_float_100 x2_100 = x1_100 * x1_100;
		cpp_dec_float_100 x3_100 = x2_100 * x1_100;
		cpp_dec_float_100 result = equation.a0;
		result += equation.a1 * x1_100;
		result += equation.a2 * x2_100;
		result += equation.a3 * x3_100;
		return result.convert_to<float>();
	}

	bool Curve::isWithinX(float x) const {
		return x >= m_range.x && x <= m_range.y;
	}

	void Curve::draw(sf::RenderWindow& window) {
		
		std::vector<sf::VertexArray> curves;
		int samples = 10;
		sf::VertexArray curve(sf::PrimitiveType::LineStrip, samples);

		for (const auto& e : m_equations) {
			float step = (e.range.y - e.range.x) / (samples - 1);
			for (int i = 0; i < samples; ++i) {
				float x = e.range.x + i * step;
				float y = getYAtX(x);
				curve[i] = sf::Vertex(sf::Vector2f(x, y), sf::Color::Cyan);
			}
			curves.push_back(curve);
		}
		for (const auto& curve : curves)
			window.draw(curve);
	}

	const curveEquation& Curve::get_equation(float x) const {
		
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

	float Curve::get_slope(float x, float distance) const {

		auto y = getYAtX(x);
		auto x2 = x + distance;
		auto y2 = getYAtX(x2);
		auto slope = (y - y2) / (x - x2);
		return slope;
	}
}