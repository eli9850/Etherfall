#include "Player.h"

#include "GameManager/GameObjects/Map.h"
#include "GameManager/ResourceManager/ResourceManager.h"


namespace Etherfall {

	Player::Player(uint32_t player_id, const std::optional<sf::Vector2f>& position) :
		m_player_id(player_id),
		m_speed(300)
	{
		const auto& player_details = g_resource_manager->get_player_details_by_id(player_id);
		const auto& player_texture = g_resource_manager->get_player_texture(player_details.at("PlayerID"));
		m_player_sprite = std::make_unique<sf::Sprite>(player_texture);
		initialize_animations(player_details.at("Sprites"));
		m_player_scale = { static_cast<float>(player_details.at("PlayerScale")[0]),
						   static_cast<float>(player_details.at("PlayerScale")[1]) };
		m_player_sprite->setScale((m_player_scale));
		m_current_animation_state = AnimationState::IdleRight;
		set_animation(Animations::Idle, 0);
		if (position) {
			m_player_sprite->setPosition(*position);
		}
		else {
			m_player_sprite->setPosition({ 150, 150 });
		}
		

		m_gravity = 1000;
		m_velocity = 0;

	}

	void Player::initialize_animations(const nlohmann::json& player_sprites) {
		
		auto& idle_animations = m_animations[Animations::Idle];
		for (const auto& idle : player_sprites.at("Idle")) {
			sf::IntRect animation_position(sf::Vector2i(idle.at("x"),idle.at("y")),
											sf::Vector2i(idle.at("width"), idle.at("height")));
			idle_animations.push_back({ animation_position });
		}
		
		auto& jump_animations = m_animations[Animations::Jump];
		for (const auto& jump : player_sprites.at("Jump")) {
			sf::IntRect animation_position(sf::Vector2i(jump.at("x"), jump.at("y")), 
										   sf::Vector2i(jump.at("width"), jump.at("height")));
			jump_animations.push_back({ animation_position });
		}

		auto& run_animations = m_animations[Animations::Run];
		for (const auto& run : player_sprites.at("Run")) {
			sf::IntRect animation_position(sf::Vector2i(run.at("x"), run.at("y")), 
										   sf::Vector2i(run.at("width"), run.at("height")));
			run_animations.push_back({ animation_position });
		}
		auto& climb_animations = m_animations[Animations::Climb];
		for (const auto& climb : player_sprites.at("Climb")) {
			sf::IntRect animation_position(sf::Vector2i(climb.at("x"), climb.at("y")),
				sf::Vector2i(climb.at("width"), climb.at("height")));
			climb_animations.push_back({ animation_position });
		}
	}

	void Player::handle_event(const std::optional<sf::Event>& event) {
		
		if (!event) {
			return;
		}
		auto key_pressed_event = event->getIf<sf::Event::KeyPressed>();
		if (key_pressed_event) {
			m_pressed_keys.insert(key_pressed_event->code);
		}
		auto key_released_event = event->getIf<sf::Event::KeyReleased>();
		if (key_released_event) {
			m_pressed_keys.erase(key_released_event->code);
		}
	}

	void Player::handle_frame(uint64_t dt, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls) {

		m_timer += dt;
		auto possible_states = get_next_possible_states();
		auto current_animation = m_current_animation_state;
		if (possible_states.size()) {
			if (handle_next_state(dt, possible_states, platforms, climbables, walls)) {
				if (current_animation != m_current_animation_state) {
					m_timer = 0;
					m_current_frame = 0;
				}
				handle_animation();
			}
			return;
		}
	}

	std::deque<PossibleState> Player::get_next_possible_states() {

		switch (m_current_animation_state)
		{
		case Etherfall::AnimationState::IdleRight:
		case Etherfall::AnimationState::IdleLeft:
			return get_next_possible_states_from_idle();
		case Etherfall::AnimationState::JumpRight:
			return get_next_possible_states_from_jump_right();
		case Etherfall::AnimationState::JumpLeft:
			return get_next_possible_states_from_jump_left();
		case Etherfall::AnimationState::RunRight:
			return get_next_possible_states_from_run_right();
		case Etherfall::AnimationState::RunLeft:
			return get_next_possible_states_from_run_left();
		case Etherfall::AnimationState::ClimbRight:
		case Etherfall::AnimationState::ClimbLeft:
			return get_next_possible_states_from_climb();
		default:
			break;
		}
		return {};
	}

	std::deque<PossibleState> Player::get_next_possible_states_from_idle() {

		std::deque<PossibleState> possible_states;

		if (m_current_animation_state == Etherfall::AnimationState::IdleRight) {
			possible_states.push_front(PossibleState::IdleRight);
		}
		if (m_current_animation_state == Etherfall::AnimationState::IdleLeft) {
			possible_states.push_front(PossibleState::IdleLeft);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::RunRight);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::RunLeft);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt)) {
			if (m_current_animation_state == AnimationState::IdleRight) {
				possible_states.push_front(PossibleState::JumpIdleRight);
			}
			else {
				possible_states.push_front(PossibleState::JumpIdleLeft);

			}
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt) &&
				 m_pressed_keys.contains(sf::Keyboard::Key::Down)) {
			possible_states.push_front(PossibleState::JumpDownRight);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Down)) {
			possible_states.push_front(PossibleState::ClimbDown);
		}

		return possible_states;
	}

    std::deque<PossibleState> Player::get_next_possible_states_from_run_right() {
		
		std::deque<PossibleState> possible_states;

		if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::RunRight);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::RunLeft);
		}
		if (!m_pressed_keys.contains(sf::Keyboard::Key::Right) && 
			!m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::IdleRight);
		}

		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt)) {
			if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
				possible_states.push_front(PossibleState::JumpRight);
			}
			else {
				possible_states.push_front(PossibleState::JumpIdleRight);
			}
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt) &&
			m_pressed_keys.contains(sf::Keyboard::Key::Down)) {
			possible_states.push_front(PossibleState::JumpDownRight);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Down) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbDown);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}

		return possible_states;
	}

	std::deque<PossibleState> Player::get_next_possible_states_from_run_left() {

		std::deque<PossibleState> possible_states;

		if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::RunLeft);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::RunRight);
		}
		if (!m_pressed_keys.contains(sf::Keyboard::Key::Right) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::IdleLeft);
		}

		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt)) {
			if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
				possible_states.push_front(PossibleState::JumpLeft);
			}
			else {
				possible_states.push_front(PossibleState::JumpIdleLeft);
			}
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt) &&
			m_pressed_keys.contains(sf::Keyboard::Key::Down)) {
			possible_states.push_front(PossibleState::JumpDownRight);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Down) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbDown);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}

		return possible_states;
	}

	std::deque<PossibleState> Player::get_next_possible_states_from_jump_right() {

		std::deque<PossibleState> possible_states;

		if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::JumpRight);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::JumpLeft);
		}
		else if (!m_pressed_keys.contains(sf::Keyboard::Key::Left) &&
				 !m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::JumpIdleRight);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}

		return possible_states;
	}

	std::deque<PossibleState> Player::get_next_possible_states_from_jump_left() {

		std::deque<PossibleState> possible_states;

		if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::JumpLeft);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::JumpRight);
		}
		else if (!m_pressed_keys.contains(sf::Keyboard::Key::Left) &&
				 !m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::JumpIdleLeft);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}

		return possible_states;
	}

	std::deque<PossibleState> Player::get_next_possible_states_from_climb() {

		std::deque<PossibleState> possible_states;

		if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt) &&
			m_pressed_keys.contains(sf::Keyboard::Key::Right) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
			possible_states.push_front(PossibleState::JumpRight);
		}
		else if (m_pressed_keys.contains(sf::Keyboard::Key::LAlt) &&
			m_pressed_keys.contains(sf::Keyboard::Key::Left) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
			possible_states.push_front(PossibleState::JumpLeft);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Down) &&
			!m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbDown);
		}
		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			possible_states.push_front(PossibleState::ClimbUp);
		}
		return possible_states;
	}

	std::optional<Platform> Player::is_on_ground(const std::vector<Platform>& platforms) {
		for (const auto& platform : platforms) {
			if (platform.isWithinX(get_position().x) && platform.getYAtX(get_position().x) == get_position().y) {
				return platform;
			}
		}
		return std::nullopt;
	}

	std::optional<Platform> Player::get_next_platform(uint64_t dt, std::vector<Platform> platforms, float distance_x, float distance_y) {
		
		auto compareX = get_position().x;

		std::sort(platforms.begin(), platforms.end(), [compareX](const Platform& a, const Platform& b) {
			return a.getYAtX(compareX) < b.getYAtX(compareX);
		});

 		for (const auto& platform : platforms) {
			if (platform.isWithinX(get_position().x + distance_x) &&
				platform.getYAtX(get_position().x) >= get_position().y + distance_y &&
				platform.getYAtX(get_position().x + distance_x) <= get_position().y + (m_velocity * dt / 1000000)) {
				return platform;
			}
		}
		return std::nullopt;
	}

	std::optional<Platform> Player::get_below_platform(std::vector<Platform> platforms) {

		auto compareX = get_position().x;

		std::sort(platforms.begin(), platforms.end(), [compareX](const Platform& a, const Platform& b) {
			return a.getYAtX(compareX) < b.getYAtX(compareX);
			});

		for (const auto& platform : platforms) {
			if (platform.isWithinX(get_position().x) &&
				platform.getYAtX(get_position().x) > get_position().y) {
				return platform;
			}
		}
		return std::nullopt;
	}

	std::optional<Wall> Player::get_next_wall(const std::vector<Wall>& walls, float distance) {

		for (const auto& wall : walls) {
			if (wall.is_through_wall(get_position(), get_width_size() / 2, distance)) {
				return wall;
			}
		}
		return std::nullopt;
	}

	bool Player::handle_next_state(uint64_t dt, const std::deque<PossibleState>& states, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls) {
		
		float distance = m_speed * dt / 1000000;
		m_velocity += m_gravity * dt / 1000000;
		for (const auto& state : states) {
			switch (state)
			{
			case Etherfall::PossibleState::IdleRight:
				if (handle_next_idle_state(dt, true, platforms, climbables)) {
					m_velocity = 0;
					return true;
				}
				break;
			case Etherfall::PossibleState::IdleLeft:
				if (handle_next_idle_state(dt, false, platforms, climbables)) {
					m_velocity = 0;
					return true;
				}
				break;
			case Etherfall::PossibleState::JumpRight: 
				if (handle_next_jump_state(dt, true, distance, platforms, climbables, walls)) {
					return true;
				}
				break;
			case Etherfall::PossibleState::JumpLeft:
				if (handle_next_jump_state(dt , false, -1 *distance, platforms, climbables, walls)) {
					return true;
				}
				break;
			case Etherfall::PossibleState::JumpIdleRight:
				if (handle_next_jump_state(dt, true, 0, platforms, climbables, walls)) {
					return true;
				}
				break;
			case Etherfall::PossibleState::JumpIdleLeft:
				if (handle_next_jump_state(dt, false, 0, platforms, climbables, walls)) {
					return true;
				}
				break;
			case Etherfall::PossibleState::RunRight: 
				if (handle_next_run_state(dt, true, distance, platforms, climbables, walls)) {
					m_velocity = 0;
					return true;
				}
				break;
			case Etherfall::PossibleState::RunLeft :
				if (handle_next_run_state(dt, false, -1 * distance, platforms, climbables, walls)) {
					m_velocity = 0;
					return true;
				}
				break;
			case Etherfall::PossibleState::ClimbUp:
				if (handle_next_climb_up_state(dt, true, -1 * distance, platforms, climbables)) {
					m_velocity = 0;
					return true;
				}
				break;
			case Etherfall::PossibleState::ClimbDown:
				if (handle_next_climb_down_state(dt, true, distance, platforms, climbables)) {
					return true;
				}
				break;
			case Etherfall::PossibleState::ClimbIdleRight:
			case Etherfall::PossibleState::ClimbIdleLeft:
				return false;
			case Etherfall::PossibleState::JumpDownRight:
			case Etherfall::PossibleState::JumpDownLeft:
				if (handle_next_jump_down_state(dt, platforms)) {
					return true;
				}
				break;
			default:
				return false;
			}
		}
		return false;
	}

	bool Player::handle_next_idle_state(uint64_t dt, bool is_right, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables) {
		
		set_current_animation_state(Animations::Idle, is_right);
		if (is_on_ground(platforms)) {
			return true;
		}
		set_current_animation_state(Animations::Jump, is_right);
		return true;
	}

	bool Player::handle_next_jump_state(uint64_t dt, bool is_right, float distance, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls) {

		if (m_current_animation_state != AnimationState::JumpRight &&
			m_current_animation_state != AnimationState::JumpLeft) {
			m_velocity = -400;
		}
		set_current_animation_state(Animations::Jump, is_right);

		if (auto wall = get_next_wall(walls, distance); wall) {
			auto distance_from_wall = get_width_size() / 2 * (is_right ? -1 : 1);
			distance = 0;
		}

		auto platform = get_next_platform(dt, platforms, distance);
		if (platform) {
			m_player_sprite->setPosition({ get_position().x + distance, platform->getYAtX(get_position().x + distance) });
			set_current_animation_state(Animations::Idle, is_right);
			return true;
		}
		m_player_sprite->move({ distance, m_velocity * dt / 1000000 });
		
		return true;
	}

	bool Player::handle_next_run_state(uint64_t dt, bool is_right, float distance, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables, const std::vector<Wall>& walls) {

		set_current_animation_state(Animations::Run, is_right);
		if (const auto platform = is_on_ground(platforms); platform) {
			if (auto wall = get_next_wall(walls, distance); wall) {
				auto distance_from_wall = get_width_size() / 2 * (is_right ? -1 : 1);
				m_player_sprite->setPosition({ wall->get_x() + distance_from_wall, platform->getYAtX(wall->get_x() + distance_from_wall) });
			}
			else {
				m_player_sprite->setPosition({ get_position().x + distance, platform->getYAtX(get_position().x + distance) });
			}
			m_velocity = 0;
			return true;
		}
		set_current_animation_state(Animations::Jump, is_right);
		return true;
	}

	bool Player::handle_next_climb_up_state(uint64_t dt, bool is_right, float distance, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables) {

		if (m_current_animation_state == AnimationState::ClimbRight) {
			for (const auto& climbable : climbables) {
				if (climbable.can_climb_up({ get_position().x, get_position().y + distance })) {
					m_player_sprite->move({ 0, distance });
					return true;
				}
			}
			if (auto platform = get_next_platform(dt, platforms, 0, distance); platform) {
				set_current_animation_state(Animations::Idle, is_right);
				m_player_sprite->setPosition({ get_position().x, platform->getYAtX(get_position().x) });
				return true;
			}
		}
		for (const auto& climbable : climbables) {
			if (climbable.can_climb_up(get_position())) {
				set_current_animation_state(Animations::Climb, is_right);
				m_player_sprite->setPosition({ climbable.get_x_position(), get_position().y });
				return true;
			}
		}
		return false;
	}

	bool Player::handle_next_climb_down_state(uint64_t dt, bool is_right, float distance, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables) {

		if (m_current_animation_state == AnimationState::ClimbRight) {
			for (const auto& climbable : climbables) {
				if (climbable.can_climb_down({ get_position().x, get_position().y + distance })) {
					m_player_sprite->move({ 0, distance });
					m_velocity = 0;
					return true;
				}
			}
			
			if (auto platform = get_next_platform(dt, platforms); platform) {
				set_current_animation_state(Animations::Idle, is_right);
				m_player_sprite->setPosition({ get_position().x, platform->getYAtX(get_position().x) });
				m_velocity = 0;
				return true;
			}
			m_player_sprite->move({ 0, distance });
			set_current_animation_state(Animations::Jump, is_right);
			m_velocity = m_speed;
			return true;
		}
		for (const auto& climbable : climbables) {
			if (climbable.can_climb_down(get_position())) {
				set_current_animation_state(Animations::Climb, is_right);
				m_player_sprite->setPosition({ climbable.get_x_position(), get_position().y });
				m_velocity = 0;
				return true;
			}
		}
		return false;
	}

	bool Player::handle_next_jump_down_state(uint64_t dt, const std::vector<Platform>& platforms) {

		auto platform = get_below_platform(platforms);
		if (platform && 
			platform->getYAtX(get_position().x) - get_position().y < 300) {
			set_current_animation_state(Animations::Jump, static_cast<uint32_t>(m_current_animation_state) % 2 == 0);
			m_player_sprite->move({ 0, 1 });
			return true;
		}
		return false;

		
	}

	void Player::handle_animation() {
		if (static_cast<uint32_t>(m_current_animation_state) % 2 != 0) {
			m_player_sprite->setScale({ m_player_scale.x * -1, m_player_scale.y });
		}
		else {
			m_player_sprite->setScale(m_player_scale);
		}
		switch (m_current_animation_state)
		{
		case Etherfall::AnimationState::IdleRight:
		case Etherfall::AnimationState::IdleLeft:
			handle_idle_animation();
			break;
		case Etherfall::AnimationState::JumpRight:
		case Etherfall::AnimationState::JumpLeft:
			handle_jump_animation();
			break;
		case Etherfall::AnimationState::RunRight:
		case Etherfall::AnimationState::RunLeft:
			handle_run_animation();
			break;
		case Etherfall::AnimationState::ClimbRight:
		case Etherfall::AnimationState::ClimbLeft:
			handle_climb_animation();
			break;
		default:
			break;
		}
	}
	
	void Player::handle_idle_animation() {

		if (m_timer > 100000) {
			m_timer %= 100000;
			const auto& animation_frames = m_animations[Animations::Idle];
			m_current_frame = (m_current_frame + 1) % animation_frames.size();
			const auto& animation_rect = animation_frames[m_current_frame];
			set_animation(static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2), m_current_frame);
		}
	}

	void Player::handle_run_animation() {
		if (m_timer > 80000) {
			m_timer %= 80000;
			const auto& animation_frames = m_animations[Animations::Run];
			m_current_frame = (m_current_frame + 1) % animation_frames.size();
			const auto& animation_rect = animation_frames[m_current_frame];
			set_animation(static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2), m_current_frame);
		}
	}

	void Player::handle_jump_animation() {
		if (m_timer > 30000) {
			m_timer %= 30000;
			const auto& animation_frames = m_animations[Animations::Jump];
			m_current_frame++;
			m_current_frame = std::min(m_current_frame, static_cast<uint32_t>(animation_frames.size() - 1));
			const auto& animation_rect = animation_frames[m_current_frame];
			set_animation(static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2), m_current_frame);
		}
	}

	void Player::handle_climb_animation() {
		if (m_timer > 80000) {
			m_timer %= 80000;
			const auto& animation_frames = m_animations[Animations::Climb];
			m_current_frame = (m_current_frame + 1) % animation_frames.size();
			const auto& animation_rect = animation_frames[m_current_frame];
			set_animation(static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2), m_current_frame);
		}
	}

	void Player::set_current_animation_state(Animations animation, bool is_right) {

		AnimationState new_state = static_cast<AnimationState>(static_cast<uint32_t>(animation) * 2 + !is_right);
		if (m_current_animation_state != new_state) {
			set_animation(static_cast<Animations>(animation));
			m_current_animation_state = new_state;
		}
	}

	void Player::set_animation(Animations animation, uint32_t frame_number) {
		const auto& new_animation = m_animations[animation][frame_number];
		m_player_sprite->setTextureRect(new_animation);
		m_player_sprite->setOrigin({ (float)new_animation.size.x / 2, (float)new_animation.size.y });
	}

	void Player::draw(sf::RenderWindow& window) {
		
		window.draw(*m_player_sprite);
	}

	sf::Vector2f Player::get_position() const {
		return m_player_sprite->getPosition();
	}

	float Player::get_height_size() const {
		return m_player_sprite->getTextureRect().size.y * m_player_scale.y;
	}

	float Player::get_width_size() const {
		return m_player_sprite->getTextureRect().size.x * m_player_scale.x;
	}
}