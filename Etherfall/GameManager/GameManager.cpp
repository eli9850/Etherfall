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
        m_window.setVerticalSyncEnabled(true);
        m_window.setKeyRepeatEnabled(false);
        m_logic = std::make_unique<LogicManager>(m_window);
    }

	void GameManager::run() {

        while (m_window.isOpen()) {
            std::optional<sf::Event> event;
            while (event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    m_window.close();
                }
                m_logic->handle_event(m_window, event);
            }
            auto dt = m_clock.restart().asMicroseconds(); 
            m_logic->handle_frame(m_window, dt);
            m_window.clear();
            m_logic->draw(m_window);
            m_window.display();
        }
	}

    void GameManager::run_test() {

        auto t = g_resource_manager->get_portal_texture(2);
        t.setRepeated(true);
        sf::Sprite a(t);
        a.setTextureRect({ {-750, -750}, {3000, 1000} });
        a.setScale({ 0.2f, 0.2f });
        a.setPosition({ 200, 200 });
        //a.setOrigin({ 116,439 });

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
                    a.move({ 5, 0 });
                }
            }

            m_window.clear();
            m_window.draw(a);
            m_window.display();
        }
    }

}
