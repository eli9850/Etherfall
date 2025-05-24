#pragma once
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include "GameManager/GameObjects/Platform.h"
#include "GameManager/GameObjects/Climbable.h"

namespace Etherfall{

	enum class Animations
	{
		Idle = 0,
		Jump,
		Run,
		Climb
	};

	enum class AnimationState
	{
		IdleRight = 0,
		IdleLeft,
		JumpRight,
		JumpLeft,
		RunRight,
		RunLeft,
		ClimbRight,
		ClimbLeft
		
	};

	class Player
	{
	public:
		Player(uint32_t player_id, sf::Vector2u map_size);
		void handle_event(const std::optional<sf::Event>& event);
		void handle_frame(uint64_t dt, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables);
		void draw(sf::RenderWindow& window);
		sf::Vector2f get_position() const;
		float get_height_size() const;
		float get_width_size() const;
	private:
		void initialize_animations(const nlohmann::json& player_sprites);
		void set_animation(Animations animation, uint32_t frame_number);
		void move_player(int64_t dt, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables);
	private:
		uint32_t m_player_id;
		sf::Vector2u m_map_size;
		std::unique_ptr<sf::Sprite> m_player_sprite;
		std::unordered_map<Animations, std::vector<sf::IntRect>> m_animations;
		sf::Vector2f m_player_scale;
		float m_speed;

		int64_t m_timer;
		uint32_t m_current_frame;
		AnimationState m_current_animation_state;
		AnimationState m_new_animation_state;
		std::set<sf::Keyboard::Key> m_pressed_keys;

		float m_gravity;
		float m_velocity;
		bool m_is_on_ground;
		bool m_is_jump;
		bool m_is_climb;
	};
}

