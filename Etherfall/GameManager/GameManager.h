#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include "GameObjects/Map.h"
#include "GameObjects/Player.h"

namespace Etherfall {
	class GameManager
	{
	public:
		GameManager();
		GameManager(const sf::Vector2u& size);
		void run();
		void run_test();
	private:
		void initialize();
	private:
		sf::RenderWindow m_window; 
		std::unique_ptr<Map> m_map;
		sf::Clock m_clock;
	};
}


