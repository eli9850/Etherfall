#include "Map.h"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

	constexpr float distance_for_height_set_view = 200;
	constexpr float distance_for_width_set_view = 250;
	constexpr float view_x_size = 800;
	constexpr float view_y_size = 600;

	Map::Map(uint32_t map_id, sf::Vector2u window_size) :
		m_window_size(std::move(window_size)),
		m_view(sf::FloatRect({ 0, 0 }, { view_x_size, view_y_size }))
	{
		const auto& map_details = g_resource_manager->get_map_details_by_id(map_id);
		const auto& background_texture = g_resource_manager->get_background_texture(map_details.at("BackgroundID"));
		m_background_sprite = std::make_unique<sf::Sprite>(background_texture);
		for (const auto& portal : map_details.at("Portals")) {
			m_portals.push_back(Portal(portal.at("PortalID"),
										   { portal.at("Pos")[0], portal.at("Pos")[1]},
											portal.at("MapID")));
		}
		for (const auto& platform : map_details.at("Platforms")) {
			m_platforms.push_back(Platform({ platform[0], platform[1] }, { platform[2], platform[3] }));
		}
		for (const auto& climbable : map_details.at("Climbables")) {
			m_climbables.push_back(Climbable(climbable.at("ClimbableID"),
												{ climbable.at("Pos")[0], climbable.at("Pos")[1] },
												  climbable.at("Size")));
		}
		for (const auto& wall : map_details.at("Walls")) {
			m_walls.push_back(Wall({ wall.at("Pos")[0], wall.at("Pos")[1] },
				wall.at("Size")));
		}
		m_background_size = background_texture.getSize();
		m_walls.push_back(Wall({ 0,0 }, static_cast<float>(m_background_size.y)));
		m_walls.push_back(Wall({ static_cast<float>(m_background_size.x),0 }, static_cast<float>(m_background_size.y)));
		m_player = std::make_unique<Player>(1);
		
	}

	uint32_t Map::handle_event(const std::optional<sf::Event>& event) {
		for (const auto& portal : m_portals) {
			if (portal.is_enter(event, m_player->get_position())) {
				return portal.get_map_id();
			}
		}
		m_player->handle_event(event);
		return 0;
	}

	void Map::handle_frame(uint64_t dt) {

		for (auto& portal : m_portals) {
			portal.handle_frame(dt);
		}
		m_player->handle_frame(dt, m_platforms, m_climbables, m_walls);
		setView();
	}

	void Map::setView() {
		auto player_position = m_player->get_position();
		sf::Vector2f center = m_view.getCenter();
		if (player_position.x - m_view.getCenter().x > distance_for_width_set_view) {
			center.x = player_position.x - distance_for_width_set_view;
		}
		if (player_position.x - m_view.getCenter().x < -distance_for_width_set_view) {
			center.x = player_position.x + distance_for_width_set_view;
		}
		if (player_position.y - m_view.getCenter().y > distance_for_height_set_view) {
			center.y = player_position.y - distance_for_height_set_view;
		}
		if (player_position.y - m_player->get_height_size() - m_view.getCenter().y < -distance_for_height_set_view) {
			center.y = player_position.y - m_player->get_height_size() + distance_for_height_set_view;
		}

		if (center.x - (view_x_size / 2) < 0 ||
			center.x + (view_x_size / 2) > m_background_size.x) {
			center.x = m_view.getCenter().x;
		}
		if (center.y - (view_y_size / 2) < 0 ||
			center.y + (view_y_size / 2) > m_background_size.y) {
			center.y = m_view.getCenter().y;
		}
		m_view.setCenter(center);
	}

	void Map::draw(sf::RenderWindow& window) {
		
		window.setView(m_view);
		window.draw(*m_background_sprite);
		for (auto& portal : m_portals) {
			portal.draw(window);
		}
		for (auto& platform : m_platforms) {
			platform.draw(window);
		}
		for (auto& climbable : m_climbables) {
			climbable.draw(window);
		}
		for (auto& wall : m_walls) {
			wall.draw(window);
		}
		m_player->draw(window);
	}
}
