#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include "Utilities/LRUCache/LRUCache.h"

namespace Etherfall {

	class ResourceManager
	{
	public:
		ResourceManager(std::filesystem::path resource_dir);
		std::filesystem::path get_maps_details_dir();
		const nlohmann::json& get_map_details_by_id(uint32_t map_id);
		std::filesystem::path get_portals_details_dir();
		const nlohmann::json& get_portal_details_by_id(uint32_t portal_id);
		std::filesystem::path get_players_details_dir();
		const nlohmann::json& get_player_details_by_id(uint32_t player_id);
		std::filesystem::path get_climbables_details_dir();
		const nlohmann::json& get_climbable_details_by_id(uint32_t climbable_id);
		const sf::Texture& get_background_texture(uint32_t background_id);
		const sf::Texture& get_portal_texture(uint32_t portal_id);
		const sf::Texture& get_player_texture(uint32_t player_id);
		sf::Texture& get_parallax_texture(uint32_t parallax_id);
	private:
		const nlohmann::json& get_json_from_file(const std::filesystem::path& json_file_path);
		std::filesystem::path get_background_path(uint32_t background_id);
		std::filesystem::path get_portal_path(uint32_t portal_id);
		std::filesystem::path get_player_path(uint32_t player_id);
		std::filesystem::path get_parallax_path(uint32_t parallax_id);
		const sf::Texture& add_background(uint32_t background_id);
		const sf::Texture& add_portal(uint32_t portal_id);
		const sf::Texture& add_player(uint32_t player_id);
		sf::Texture& add_parallax(uint32_t parallax_id);
	private:
		std::filesystem::path m_resource_dir;
		Utilities::LRUCache<uint32_t, sf::Texture> m_background_textures;
		Utilities::LRUCache<uint32_t, sf::Texture> m_portal_textures;
		Utilities::LRUCache<uint32_t, sf::Texture> m_player_textures;
		Utilities::LRUCache<uint32_t, sf::Texture> m_parallax_textures;
		Utilities::LRUCache<std::filesystem::path, nlohmann::json> m_objects_details;
	};

	extern std::unique_ptr<ResourceManager> g_resource_manager;
}
