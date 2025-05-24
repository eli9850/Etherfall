#include "GameManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "ResourceManager/ResourceManager.h"

namespace Etherfall {

    GameManager::GameManager() :
        m_window(sf::VideoMode::getDesktopMode(), "Etherfall", sf::State::Fullscreen)
    {
        initialize();
    }

    GameManager::GameManager(const sf::Vector2u& size) :
        m_window(sf::VideoMode(size), "Etherfall")
    {
        initialize();
    }

    void GameManager::initialize() {
        m_window.setFramerateLimit(60);
        m_window.setKeyRepeatEnabled(false);
        m_map = std::make_unique<Map>(1, m_window.getSize());
    }

	void GameManager::run() {

        while (m_window.isOpen()) {
            std::optional<sf::Event> event;
            while (event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    m_window.close();
                }        
                if (m_window.hasFocus()) {
                    auto map = m_map->handle_event(event);
                    if (map != 0) {
                        m_map = std::make_unique<Map>(map, m_window.getSize());
                    }
                }            
            }
            auto dt = m_clock.restart().asMicroseconds();
            
            m_map->handle_frame(dt);
            m_window.clear();
            m_map->draw(m_window);
            m_window.display();
        }
	}

    void GameManager::run_test() {

        auto t = g_resource_manager->get_player_texture(1);
        sf::Sprite a(t);
        a.setTextureRect({ {162, 2219}, {232, 439} });
        a.setScale({ 0.2f, 0.2f });
        a.setPosition({ 300, 300 });
        a.setOrigin({ 116,439 });

        while (m_window.isOpen()) {
            std::optional<sf::Event> event;
            while (event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    m_window.close();
                }
                if (event->is<sf::Event::KeyPressed>()) {
                    a.setScale({ a.getScale().x * -1, a.getScale().y });
                    std::cout << a.getPosition().x << " | " << a.getPosition().y << std::endl;
                    std::cout << a.getScale().x << " | " << a.getScale().y << std::endl;
                }
            }

            m_window.clear();
            m_window.draw(a);
            m_window.display();
        }
    }

}
