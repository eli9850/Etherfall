#pragma once
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <optional>
#include "GameManager/GameObjects/Walkable.h"
#include "GameManager/GameObjects/Climbable.h"
#include "GameManager/GameObjects/Wall.h"

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

	enum class PossibleState
	{
		IdleRight = 0,
		IdleLeft,
		JumpIdleRight,
		JumpIdleLeft,
		JumpRight,
		JumpLeft,
		JumpDownRight,
		JumpDownLeft,
		RunRight,
		RunLeft,
		ClimbIdleRight,
		ClimbIdleLeft,
		ClimbUp,
		ClimbDown
	};

	class Player
	{
	public:
		Player(uint32_t player_id, const std::optional<sf::Vector2f>& position);
		void handle_event(const std::optional<sf::Event>& event);
		void handle_frame(uint64_t dt, const std::vector<std::shared_ptr<Walkable>>& walkables, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls);
		void draw(sf::RenderWindow& window);
		sf::Vector2f get_position() const;
		float get_height_size() const;
		float get_width_size() const;
	private:
		void initialize_animations(const nlohmann::json& player_sprites);

		std::deque<PossibleState> get_next_possible_states();
		std::deque<PossibleState> get_next_possible_states_from_idle();
		std::deque<PossibleState> get_next_possible_states_from_run_right();
		std::deque<PossibleState> get_next_possible_states_from_run_left();
		std::deque<PossibleState> get_next_possible_states_from_jump_right();
		std::deque<PossibleState> get_next_possible_states_from_jump_left();
		std::deque<PossibleState> get_next_possible_states_from_climb();

		std::shared_ptr<Walkable> is_on_ground(const std::vector<std::shared_ptr<Walkable>>& walkable);
		// TODO: I need to find a way to fix the sort issue
		std::shared_ptr<Walkable> get_next_walkable(uint64_t dt, std::vector<std::shared_ptr<Walkable>> walkable, float distance_x = 0, float distance_y = 0);
		std::shared_ptr<Walkable> get_below_walkable(std::vector<std::shared_ptr<Walkable>> walkable);
		std::optional<Wall> get_next_wall(const std::vector<Wall>& walls, float distance);
		bool handle_next_state(uint64_t dt, const std::deque<PossibleState>& states, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls);
		bool handle_next_idle_state(uint64_t dt, bool is_right, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables);
		bool handle_next_jump_state(uint64_t dt, bool is_right, float distance, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls);
		bool handle_next_run_state(uint64_t dt, bool is_right, float distance, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls);
		bool handle_next_climb_up_state(uint64_t dt, bool is_right, float distance, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables);
		bool handle_next_climb_down_state(uint64_t dt, bool is_right, float distance, const std::vector<std::shared_ptr<Walkable>>& walkable, const std::vector<Climbable>& climbables);
		bool handle_next_jump_down_state(uint64_t dt, const std::vector<std::shared_ptr<Walkable>>& walkable);
		
		void handle_animation();
		void handle_idle_animation();
		void handle_run_animation();
		void handle_jump_animation();
		void handle_climb_animation();
		void set_current_animation_state(Animations animation, bool is_right);
		void set_animation(Animations animation, uint32_t frame_number = 0);
		
	private:
		uint32_t m_player_id;
		std::unique_ptr<sf::Sprite> m_player_sprite;
		std::unordered_map<Animations, std::vector<sf::IntRect>> m_animations;
		sf::Vector2f m_player_scale;
		float m_speed;
		float m_gravity;
		uint32_t m_current_frame;
		AnimationState m_current_animation_state;
		float m_velocity;
		int64_t m_timer;
		std::set<sf::Keyboard::Key> m_pressed_keys;
	};
}

