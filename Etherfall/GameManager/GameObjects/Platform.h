#pragma once

#include <SFML/Graphics.hpp>
#include "Walkable.h"

namespace Etherfall {

	struct platformEquation {
		float a0;
		float a1;
		sf::Vector2f range;
	};

	class Platform final : public Walkable
	{
	public:
		Platform(std::vector<platformEquation> platform);
		void draw(sf::RenderWindow& window) override;
		float getYAtX(float x) const override;
		bool isWithinX(float x) const override;
		float get_slope(float x, float distance) const override;
	private:
		// TODO: change this to binary search
		const platformEquation& get_equation(float x) const;
		
	private:
		sf::Vector2f m_range;
		std::vector<platformEquation> m_equations;
	};
}
