#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <vector>
#include "LogicManager.h"
#include "GameObjects/Map.h"
#include "GameObjects/Player.h"
#include "GameManager/UIObjects/Dialogue.h"

namespace Etherfall {
	class LogicManager
	{
	public:
		LogicManager(const sf::RenderWindow& window);
		void handle_event(const sf::RenderWindow& window, const std::optional<sf::Event>& event);
		void handle_frame(const sf::RenderWindow& window, uint64_t dt);
		void draw(sf::RenderWindow& window);
	private:
		void handle_mouse_pressed_event(const sf::RenderWindow& window, const sf::Event::MouseButtonPressed& event);
		bool is_map_focus() const;
	private:
		sf::Vector2u m_window_size;
		sf::View m_logic_view;
		std::shared_ptr<Player> m_player;
		std::shared_ptr<Map> m_map;
		Dialogue m_dialuge;
	};
}


