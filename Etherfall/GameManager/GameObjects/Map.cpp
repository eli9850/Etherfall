#include "Map.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <Windows.h>
#include "GameManager/ResourceManager/ResourceManager.h"
#include "Platform.h"
#include "Curve.h"

namespace Etherfall {

	constexpr float distance_for_height_set_view = 200;
	constexpr float distance_for_width_set_view = 250;
	constexpr float view_x_size = 800;
	constexpr float view_y_size = 600;

	Map::Map(uint32_t map_id, sf::Vector2u window_size, const std::optional<uint32_t>& previous_map) :
		m_map_id(map_id),
		m_window_size(std::move(window_size)),
		m_view(sf::FloatRect({ 0, 0 }, { view_x_size, view_y_size }))
	{
		const auto& map_details = g_resource_manager->get_map_details_by_id(m_map_id);
		const auto& background_texture = g_resource_manager->get_background_texture(map_details.at("BackgroundID"));
		m_background_size = background_texture.getSize();
		m_background_sprite = std::make_unique<sf::Sprite>(background_texture);
		m_parallax = std::make_unique<Parallax>(map_details.at("Parallax"), m_background_size);
		std::optional<sf::Vector2f> player_position = std::nullopt;
		for (const auto& portal : map_details.at("Portals")) {
			auto portal_map_id = portal.at("MapID");
			Portal p(portal.at("PortalID"), { portal.at("Pos")[0], portal.at("Pos")[1] }, portal_map_id);
			if (previous_map.has_value() && portal_map_id == *previous_map) {
				player_position = p.get_enter_position();
			}
			m_portals.push_back(std::move(p));
			
		}
		for (const auto& curve : map_details.at("Curves")) {
			std::vector<curveEquation> equations;
			for (const auto& equation : curve) {
				equations.push_back({ equation[0], equation[1], equation[2], equation[3], {equation[4], equation[5] } });
			}
			auto real_curve = std::make_shared<Curve>(std::move(equations));
			m_walkables.push_back(std::move(real_curve));
		}
		for (const auto& platform : map_details.at("Platforms")) {
			std::vector<platformEquation> equations;
			for (const auto& equation : platform) {
				equations.push_back({ equation[0], equation[1], {equation[2], equation[3] } });
			}
			auto real_platform = std::make_shared<Platform>(std::move(equations));
			m_walkables.push_back(std::move(real_platform));
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
		m_walls.push_back(Wall({ 0,0 }, static_cast<float>(m_background_size.y)));
		m_walls.push_back(Wall({ static_cast<float>(m_background_size.x),0 }, static_cast<float>(m_background_size.y)));
		m_player = std::make_unique<Player>(1, player_position);
		
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
		m_player->handle_frame(dt, m_walkables, m_climbables, m_walls);
		auto old_center = m_view.getCenter().x;
		setView();
		m_parallax->update(m_view.getCenter().x - old_center);
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
		m_parallax->draw(window);
		window.draw(*m_background_sprite);
		for (auto& portal : m_portals) {
			portal.draw(window);
		}
		/*for (auto& walkable : m_walkables) {
			walkable->draw(window);
		}
		for (auto& climbable : m_climbables) {
			climbable.draw(window);
		}*/
		for (auto& wall : m_walls) {
			wall.draw(window);
		}
		m_player->draw(window);
	}

	uint32_t Map::get_map_id() const {
		return m_map_id;
	}
}
