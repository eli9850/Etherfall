#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <memory>
#include <vector>
#include <optional>
#include "Parallax.h"
#include "Portal.h"
#include "Player.h"
#include "Walkable.h"
#include "Climbable.h"
#include "Wall.h"


namespace Etherfall {
	class Map
	{
	public:
		Map(uint32_t map_id, sf::Vector2u window_size, const std::optional<uint32_t>& previous_map);
		uint32_t handle_event(const std::optional<sf::Event>& event);
		void handle_frame(uint64_t dt);
		void draw(sf::RenderWindow& window);
		uint32_t get_map_id() const;

	private:
		void setView();

	private:
		uint32_t m_map_id;
		sf::Vector2u m_window_size;
		sf::Vector2u m_background_size;
		std::unique_ptr<sf::Sprite> m_background_sprite;
		std::unique_ptr<Parallax> m_parallax;
		std::vector<Portal> m_portals;
		std::vector<std::shared_ptr<Walkable>> m_walkables;
		std::vector<Climbable> m_climbables;
		std::vector<Wall> m_walls;
		std::unique_ptr<Player> m_player;
		sf::View m_view;
	};
}

