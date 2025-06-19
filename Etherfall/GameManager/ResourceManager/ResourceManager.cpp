#include "ResourceManager.h"

#include <string>
#include <format>
#include <fstream>

namespace Etherfall {

	ResourceManager::ResourceManager(std::filesystem::path resource_dir) 
		: m_resource_dir(std::move(resource_dir)), 
		m_background_textures(10), 
		m_portal_textures(3),  
		m_player_textures(3),
		m_parallax_textures(20),
		m_npc_textures(30),
		m_objects_details(50)
		{}

	std::filesystem::path ResourceManager::get_maps_details_dir() {
		static auto map_dir = m_resource_dir / "Details" / "Maps";
		return map_dir;
	}

	const nlohmann::json& ResourceManager::get_map_details_by_id(uint32_t map_id) {
		const auto map_details_path = get_maps_details_dir() / (std::string("Map") + std::to_string(map_id) + ".json");
		return get_json_from_file(map_details_path);	
	}

	std::filesystem::path ResourceManager::get_portals_details_dir() {
		static auto portal_dir = m_resource_dir / "Details" / "Portals";
		return portal_dir;
	}

	const nlohmann::json& ResourceManager::get_portal_details_by_id(uint32_t portal_id) {
		const auto portal_details_path = get_portals_details_dir() / (std::string("Portal") + std::to_string(portal_id) + ".json");
		return get_json_from_file(portal_details_path);
	}

	std::filesystem::path ResourceManager::get_players_details_dir() {
		static auto player_dir = m_resource_dir / "Details" / "Players";
		return player_dir;
	}

	const nlohmann::json& ResourceManager::get_player_details_by_id(uint32_t player_id) {
		const auto player_details_path = get_players_details_dir() / (std::string("Player") + std::to_string(player_id) + ".json");
		return get_json_from_file(player_details_path);
	}

	std::filesystem::path ResourceManager::get_climbables_details_dir() {
		static auto climbable_dir = m_resource_dir / "Details" / "Climbables";
		return climbable_dir;
	}

	const nlohmann::json& ResourceManager::get_climbable_details_by_id(uint32_t climbable_id) {
		const auto climbable_details_path = get_climbables_details_dir() / (std::string("Climbable") + std::to_string(climbable_id) + ".json");
		return get_json_from_file(climbable_details_path);
	}

	std::filesystem::path ResourceManager::get_npcs_details_dir() {
		static auto climbable_dir = m_resource_dir / "Details" / "NPCs";
		return climbable_dir;
	}

	const nlohmann::json& ResourceManager::get_npc_details_by_id(uint32_t npc_id) {
		const auto npc_details_path = get_npcs_details_dir() / (std::string("NPC") + std::to_string(npc_id) + ".json");
		return get_json_from_file(npc_details_path);
	}

	const nlohmann::json& ResourceManager::get_json_from_file(const std::filesystem::path& json_file_path) {
		std::ifstream map_details_file(json_file_path);
		if (!map_details_file) {
			throw std::runtime_error(std::format("Could not open file {}", json_file_path.string()));
		}
		try {
			m_objects_details.put(json_file_path, nlohmann::json::parse(map_details_file));
			return m_objects_details.get(json_file_path);
		}
		catch (const nlohmann::json::parse_error&) {
			throw std::runtime_error(std::format("Could not parse file {}", json_file_path.string()));
		}
	}
	
	std::filesystem::path ResourceManager::get_background_path(uint32_t background_id) {
		static auto background_path = m_resource_dir / "Backgrounds";
		return background_path / (std::string("Background") + std::to_string(background_id) + ".png");
	}

	std::filesystem::path ResourceManager::get_portal_path(uint32_t portal_id) {
		static auto portal_path = m_resource_dir / "Portals";
		return portal_path / (std::string("Portal") + std::to_string(portal_id) + ".png");
	}

	std::filesystem::path ResourceManager::get_player_path(uint32_t player_id) {
		static auto player_path = m_resource_dir / "Players";
		return player_path / (std::string("Player") + std::to_string(player_id) + ".png");
	}

	std::filesystem::path ResourceManager::get_parallax_path(uint32_t parallax_id) {
		static auto parallax_path = m_resource_dir / "Parallaxs";
		return parallax_path / (std::string("Parallax") + std::to_string(parallax_id) + ".png");
	}

	std::filesystem::path ResourceManager::get_npc_path(uint32_t npc_id) {
		static auto npc_path = m_resource_dir / "NPCs";
		return npc_path / (std::string("NPC") + std::to_string(npc_id) + ".png");
	}

	const sf::Texture& ResourceManager::add_background(uint32_t background_id) {
		sf::Texture background;
		const auto background_path = get_background_path(background_id);
		if (!background.loadFromFile(background_path)) {
			throw std::runtime_error(std::format("Could not load texture from file {}", background_path.string()));
		}
		m_background_textures.put(background_id, std::move(background));
		return m_background_textures.get(background_id);
	}

	const sf::Texture& ResourceManager::add_portal(uint32_t portal_id) {
		sf::Texture portal;
		const auto portal_path = get_portal_path(portal_id);
		if (!portal.loadFromFile(portal_path)) {
			throw std::runtime_error(std::format("Could not load texture from file {}", portal_path.string()));
		}
		m_portal_textures.put(portal_id, std::move(portal));
		return m_portal_textures.get(portal_id);
	}

	const sf::Texture& ResourceManager::add_player(uint32_t player_id) {
		sf::Texture player;
		const auto player_path = get_player_path(player_id);
		if (!player.loadFromFile(player_path)) {
			throw std::runtime_error(std::format("Could not load texture from file {}", player_path.string()));
		}
		m_player_textures.put(player_id, std::move(player));
		return m_player_textures.get(player_id);
	}

	sf::Texture& ResourceManager::add_parallax(uint32_t parallax_id) {
		sf::Texture parallax;
		const auto parallax_path = get_parallax_path(parallax_id);
		if (!parallax.loadFromFile(parallax_path)) {
			throw std::runtime_error(std::format("Could not load texture from file {}", parallax_path.string()));
		}
		m_parallax_textures.put(parallax_id, std::move(parallax));
		return m_parallax_textures.get(parallax_id);
	}

	const sf::Texture& ResourceManager::add_npc(uint32_t npc_id) {
		sf::Texture npc;
		const auto npc_path = get_npc_path(npc_id);
		if (!npc.loadFromFile(npc_path)) {
			throw std::runtime_error(std::format("Could not load texture from file {}", npc_path.string()));
		}
		m_npc_textures.put(npc_id, std::move(npc));
		return m_npc_textures.get(npc_id);
	}

	const sf::Texture& ResourceManager::get_background_texture(uint32_t background_id) {
		if (!m_background_textures.exists(background_id)) {
			return add_background(background_id);
		}
		return m_background_textures.get(background_id);
	}

	const sf::Texture& ResourceManager::get_portal_texture(uint32_t portal_id) {
		if (!m_portal_textures.exists(portal_id)) {
			return add_portal(portal_id);
		}
		return m_portal_textures.get(portal_id);
	}

	const sf::Texture& ResourceManager::get_player_texture(uint32_t player_id) {
		if (!m_player_textures.exists(player_id)) {
			return add_player(player_id);
		}
		return m_player_textures.get(player_id);
	}

	sf::Texture& ResourceManager::get_parallax_texture(uint32_t parallax_id) {
		if (!m_parallax_textures.exists(parallax_id)) {
			return add_parallax(parallax_id);
		}
		return m_parallax_textures.get(parallax_id);
	}

	const sf::Texture& ResourceManager::get_npc_texture(uint32_t npc_id) {
		if (!m_npc_textures.exists(npc_id)) {
			return add_npc(npc_id);
		}
		return m_npc_textures.get(npc_id);
	}

	std::unique_ptr<ResourceManager> g_resource_manager = nullptr;
}


