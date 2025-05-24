#include "Player.h"

#include "GameManager/GameObjects/Map.h"
#include "GameManager/ResourceManager/ResourceManager.h"


namespace Etherfall {

	Player::Player(uint32_t player_id, sf::Vector2u map_size) :
		m_player_id(player_id),
		m_map_size(std::move(map_size)),
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
		m_new_animation_state = AnimationState::IdleRight;
		set_animation(Animations::Idle, 0);
		m_player_sprite->setPosition({ 150, 150 });
		
		m_gravity = 500;
		m_velocity = 0;
		m_is_on_ground = true;
		m_is_jump = false;
		m_is_climb = false;
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

	void Player::set_animation(Animations animation, uint32_t frame_number) {
		const auto& new_animation = m_animations[animation][frame_number];
		m_player_sprite->setTextureRect(new_animation);
		m_player_sprite->setOrigin({ (float)new_animation.size.x / 2, (float)new_animation.size.y });
	}

	void Player::handle_event(const std::optional<sf::Event>& event) {
		
		if (!event) {
			return;
		}
		auto key_pressed_event = event->getIf<sf::Event::KeyPressed>();
		if (key_pressed_event) {
			m_pressed_keys.insert(key_pressed_event->code);
			if (key_pressed_event->code == sf::Keyboard::Key::Right && 
				!m_pressed_keys.contains(sf::Keyboard::Key::Left) &&
				!m_is_climb)
			{
				if (m_is_jump) {
					m_new_animation_state = AnimationState::JumpRight;
				}
				else {
					m_new_animation_state = AnimationState::RunRight;
				}
				
			}
			else if (key_pressed_event->code == sf::Keyboard::Key::Left &&
					!m_pressed_keys.contains(sf::Keyboard::Key::Right) &&
					!m_is_climb)
			{
				if (m_is_jump) {
					m_new_animation_state = AnimationState::JumpLeft;
				}
				else {
					m_new_animation_state = AnimationState::RunLeft;
				}
			}
			else if (key_pressed_event->code == sf::Keyboard::Key::LAlt && !m_is_jump && (m_is_on_ground || m_is_climb)) {
				m_is_jump = true;
				m_is_on_ground = false; 
				m_velocity = -300;
				m_is_climb = false;
				if (static_cast<uint32_t>(m_current_animation_state) % 2 == 0) {
					m_new_animation_state = AnimationState::JumpRight;	
				}
				else {
					m_new_animation_state = AnimationState::JumpLeft;
				}
				
			}
		}
		auto key_released_event = event->getIf<sf::Event::KeyReleased>();
		if (key_released_event) {
			m_pressed_keys.erase(key_released_event->code);
			if (key_released_event->code == sf::Keyboard::Key::Right &&
				m_current_animation_state != AnimationState::RunLeft)
			{
				if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
					m_new_animation_state = AnimationState::RunLeft;
				}
				else {
					m_new_animation_state = AnimationState::IdleRight;
				}
				
			}
			else if (key_released_event->code == sf::Keyboard::Key::Left &&
				m_current_animation_state != AnimationState::RunRight)
			{
				if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
					m_new_animation_state = AnimationState::RunRight;
				}
				else {
					m_new_animation_state = AnimationState::IdleLeft;
				}
			}
		}
	}

	void Player::handle_frame(uint64_t dt, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables) {

		m_timer += dt;
		bool is_animation_change = false;
		if (m_current_animation_state != m_new_animation_state) {
			m_current_animation_state = m_new_animation_state;
			m_timer = 0;
			m_current_frame = 0;
			is_animation_change = true;
			if (static_cast<uint32_t>(m_current_animation_state) % 2 != 0) {
				m_player_sprite->setScale({ m_player_scale.x * -1, m_player_scale.y });
			}
			else {
				m_player_sprite->setScale(m_player_scale);
			}
		}
		
		if (m_timer > 80000 || is_animation_change) {
			m_timer %= 80000;
			const auto& animation_frames = m_animations[static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2)];
			m_current_frame = (m_current_frame + 1) % animation_frames.size();
			const auto& animation_rect = animation_frames[m_current_frame];
			set_animation(static_cast<Animations>(static_cast<uint32_t>(m_current_animation_state) / 2), m_current_frame);
		}
		
		move_player(dt, platforms, climbables);
	}

	void Player::move_player(int64_t dt, const std::vector<Platform>& platforms, const std::vector<Climbable>& climbables) {

		std::cout << get_position().x << " | " << get_position().y << std::endl;

		float distance = m_speed * dt / 1000000;

		if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
			for (const auto& climbable : climbables) {
				if (climbable.can_climb({ get_position().x, get_position().y - m_player_sprite->getTextureRect().size.y / 2 })) {
					m_new_animation_state = AnimationState::ClimbRight;
					m_is_jump = false;
					m_is_on_ground = false;
					m_is_climb = true;
					m_player_sprite->setPosition({ climbable.get_x_position(), get_position().y });
				}
			}
		}

		if (m_is_climb) {
			if (m_pressed_keys.contains(sf::Keyboard::Key::Up)) {
				m_player_sprite->move({ 0, -distance });
			}
			if (m_pressed_keys.contains(sf::Keyboard::Key::Down)) {
				m_player_sprite->move({ 0, distance });
			}
			return;
		}

		m_is_on_ground = false;
		m_velocity += m_gravity * dt / 1000000;
		for (const auto& platform : platforms) {
			float playerX = get_position().x;
			if (platform.isWithinX(playerX)) {
				float groundY = platform.getYAtX(playerX);
				float playerFootY = get_position().y;

				if (playerFootY <= groundY && playerFootY + m_velocity * dt / 1000000 > groundY) {
					m_player_sprite->setPosition({ playerX, groundY });
					m_velocity = 0;
					m_is_on_ground = true;
					m_is_jump = false;
					if (m_current_animation_state == AnimationState::JumpRight) {
						if (m_pressed_keys.contains(sf::Keyboard::Key::Right)) {
							m_new_animation_state = AnimationState::RunRight;
						}
						else {
							m_new_animation_state = AnimationState::IdleRight;
						}
					}
					else if (m_current_animation_state == AnimationState::JumpLeft) {
						if (m_pressed_keys.contains(sf::Keyboard::Key::Left)) {
							m_new_animation_state = AnimationState::RunLeft;
						}
						else {
							m_new_animation_state = AnimationState::IdleLeft;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
						get_position().x < m_map_size.x - get_width_size() / 2) {
						m_player_sprite->setPosition({ playerX + distance, platform.getYAtX(playerX + distance) });
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
						get_position().x > get_width_size() / 2) {
						m_player_sprite->setPosition({ playerX - distance, platform.getYAtX(playerX - distance) });
					}
					return;
					
				}
			}
		}
		m_player_sprite->move({ 0, (float)m_velocity * dt / 1000000 });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
			get_position().x < m_map_size.x - get_width_size() / 2) {
			m_player_sprite->move({ distance, 0 });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
			get_position().x > get_width_size() / 2) {
			m_player_sprite->move({ -distance, 0 });
		}
		
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