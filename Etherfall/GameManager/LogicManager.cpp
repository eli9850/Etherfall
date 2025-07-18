#include "LogicManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "ResourceManager/ResourceManager.h"

namespace Etherfall {

    constexpr float view_x_size = 800;
    constexpr float view_y_size = 600;

    LogicManager::LogicManager(const sf::RenderWindow& window) : 
        m_window_size(window.getSize()),
        m_logic_view(sf::FloatRect({ 0, 0 }, { view_x_size, view_y_size }))
    {
        m_player = std::make_shared<Player>(1);
        m_map = std::make_shared<Map>(1, m_window_size, m_player, std::nullopt);
    }

    bool LogicManager::is_map_focus() const  {
        if (m_dialuge.get_is_visiable()) {
            return false;
        }
        return true;
    }

    void LogicManager::handle_event(const sf::RenderWindow& window, const std::optional<sf::Event>& event) {

        auto mouse_pressed_event = event->getIf<sf::Event::MouseButtonPressed>();
        if (mouse_pressed_event && mouse_pressed_event->button == sf::Mouse::Button::Left) {
            handle_mouse_pressed_event(window, *mouse_pressed_event);
        }

        if (is_map_focus()) {
            auto map = m_map->handle_event(event);
            if (map != 0) {
                m_map = std::make_shared<Map>(map, m_window_size, m_player, m_map->get_map_id());
            }
        }
    }

    void LogicManager::handle_mouse_pressed_event(const sf::RenderWindow& window, const sf::Event::MouseButtonPressed& event) {
        
        if (m_dialuge.get_is_visiable()) {
            m_dialuge.handle_mouse_pressed_event(window, event);
        }
        else {
            auto npc = m_map->get_clicked_npc(window, event.position);
            if (npc) {
                m_player->reset_events();
                m_dialuge.set_dialogue(*npc);
            }
        }
    }

    void LogicManager::handle_frame(const sf::RenderWindow& window, uint64_t dt) {

        m_map->handle_frame(dt);
        if (m_dialuge.get_is_visiable()) {
            m_dialuge.handle_frame(window, dt);
        }
    }

    void LogicManager::draw(sf::RenderWindow& window) {
        m_map->draw(window);
        window.setView(m_logic_view);
        if (m_dialuge.get_is_visiable()) {
            m_dialuge.draw(window);
        }
    }  
}
