#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Button.h"
#include "GameManager/GameObjects/NPC.h"

namespace Etherfall {

	class Dialogue
	{
	public:
		Dialogue();
		bool get_is_visiable() const;
		void set_dialogue(const NPC& npc);
		void handle_mouse_pressed_event(const sf::RenderWindow& window, const sf::Event::MouseButtonPressed& event);
		void handle_frame(const sf::RenderWindow& window, uint64_t dt);
		void draw(sf::RenderWindow& window);
	private:
		void handle_text_frame();
		void handle_mouse_frame(const sf::Vector2f& pos);
	private:
		bool m_is_visiable;
		uint32_t m_page;
		std::vector<std::string> m_text;
		SFML::RoundedRectangle m_background;
		SFML::RoundedRectangle m_npc_background;
		sf::RectangleShape m_npc_name_shape;
		sf::Text m_npc_name;
		std::unique_ptr<sf::Sprite>	m_npc;
		SFML::RoundedRectangle m_text_background;
		sf::Text m_npc_text;
		uint64_t m_timer;
		Button m_end_dialogue;
		Button m_next_page;
		Button m_previous_page;

	};
}

