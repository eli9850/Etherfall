#include "Button.h"

#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	Button::Button(std::string text,
		uint32_t text_size,
		const sf::Vector2f& size,
		const sf::Color& topColor,
		const sf::Color& bottomColor,
		float radius,
		unsigned int cornerPoints) :
		m_text(g_resource_manager->get_font(), std::move(text), text_size),
		m_rect(size, topColor, bottomColor, radius, cornerPoints)
	{
		sf::FloatRect bounds = m_text.getGlobalBounds();
		m_text.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
	}

	bool Button::is_hover(const sf::Vector2f& position) {
		return m_rect.getGlobalBounds().contains(position);
	}

	void Button::set_position(const sf::Vector2f& position) {
		m_rect.setPosition(position);
		m_text.setPosition({ position.x + m_rect.getGlobalBounds().size.x / 2, position.y + m_rect.getGlobalBounds().size.y / 2 });
	}

	void Button::set_text_color(const sf::Color& color) {
		m_text.setFillColor(color);
	}

	void Button::draw(sf::RenderWindow& window) {
		window.draw(m_rect);
		window.draw(m_text);
	}
}