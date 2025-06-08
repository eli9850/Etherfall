#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Walkable.h"

namespace Etherfall {

	struct equation {
		float a0;
		float a1; 
		float a2; 
		float a3;
		sf::Vector2f range;
	};

	class Curve final : public Walkable
	{
	public:
		Curve(std::vector<equation> curve);
		void draw(sf::RenderWindow& window) override;
		float getYAtX(float x) const override;
		bool isWithinX(float x) const override;
	private:
		// TODO: change this to binary search
		const equation& get_equation(float x) const;
	private:
		sf::Vector2f m_range;
		std::vector<equation> m_equations;
	};
}
