#pragma once

#include <SFML/Graphics.hpp>
#include "Walkable.h"

namespace Etherfall {

	class Platform final : public Walkable
	{
	public:
		Platform(sf::Vector2f start, sf::Vector2f end);
		void draw(sf::RenderWindow& window) override;
		float getYAtX(float x) const override;
		bool isWithinX(float x) const override;
	private:
		sf::Vector2f m_start;
		sf::Vector2f m_end;
		float m_slope;
		float m_intercept;
	};
}
