#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Walkable.h"

namespace Etherfall {

	struct curveEquation {
		double a0;
		double a1;
		double a2;
		double a3;
		sf::Vector2f range;
	};

	class Curve final : public Walkable
	{
	public:
		Curve(std::vector<curveEquation> curve);
		void draw(sf::RenderWindow& window) override;
		float getYAtX(float x) const override;
		bool isWithinX(float x) const override;
		float get_slope(float x, float distance) const override;
	private:
		// TODO: change this to binary search
		const curveEquation& get_equation(float x) const;
	private:
		sf::Vector2f m_range;
		std::vector<curveEquation> m_equations;
	};
}
