#include "Dialogue.h"

#include <iostream>
#include "GameManager/ResourceManager/ResourceManager.h"

namespace Etherfall {

    constexpr float x_pos = 100;
    constexpr float y_pos = 150;

    Dialogue::Dialogue() : m_is_visiable(false), m_page(0),
        m_background{ { 600, 300 }, sf::Color(163, 168, 126, 150), sf::Color(255, 255, 255, 150) },
        m_npc_background{ { 150, 240 }, sf::Color::White, sf::Color::White },
        m_npc_name(g_resource_manager->get_font()),
        m_npc(std::make_unique<sf::Sprite>(g_resource_manager->get_npc_texture(1))),
        m_text_background{ { 410, 240 }, sf::Color::White, sf::Color::White },
        m_npc_text(g_resource_manager->get_font()),
        m_timer(0),
        m_end_dialogue{"end chat", 14, { 100, 30 }, sf::Color(196, 156, 26), sf::Color(196, 156, 26, 150), 4 },
        m_next_page{ "next ->", 14, { 70, 30 }, sf::Color(37, 162, 179), sf::Color(37, 162, 179, 150), 4 },
        m_previous_page{ "<- previous", 14, { 80, 30 }, sf::Color(37, 162, 179), sf::Color(37, 162, 179, 150), 4 }
    {
        m_background.setPosition({ x_pos, y_pos });

        m_npc_background.setPosition({ x_pos + 10, y_pos + 10 });
        m_npc->setPosition({ x_pos + 15, y_pos + 35 });
        m_npc_name_shape.setSize({ 140, 25 });
        m_npc_name_shape.setFillColor(sf::Color(62, 225,233, 150));
        m_npc_name_shape.setOutlineThickness(2);
        m_npc_name_shape.setOutlineColor(sf::Color::Black);
        m_npc_name_shape.setPosition({ x_pos + 15, y_pos + 170 });
        m_npc_name.setCharacterSize(18);
        m_npc_name.setFillColor(sf::Color::Black);
        m_npc_name.setPosition({ m_npc_name_shape.getPosition().x + m_npc_name_shape.getLocalBounds().size.x / 2, m_npc_name_shape.getPosition().y + m_npc_name_shape.getLocalBounds().size.y / 2 });

        m_text_background.setPosition({ x_pos + 170, y_pos + 10 });
        m_npc_text.setCharacterSize(14);
        m_npc_text.setFillColor(sf::Color::Black);
        m_npc_text.setPosition({ x_pos + 175, y_pos + 15 });

        m_end_dialogue.set_position({ 10 + x_pos, 260 + y_pos });
        m_next_page.set_position({ 520 + x_pos, 260 + y_pos });
        m_previous_page.set_position({ 430 + x_pos , 260 + y_pos });
	}

    bool Dialogue::get_is_visiable() const {
        return m_is_visiable;
    }

    void Dialogue::set_dialogue(const NPC& npc) {
        m_is_visiable = true;
        m_page = 0;
        m_text = npc.get_dialogue();
        const auto& npc_texture = g_resource_manager->get_npc_texture(npc.get_id());
        m_npc->setTexture(npc_texture);
        m_npc->setScale(sf::Vector2f({ 130.f / npc_texture.getSize().x, 130.f / npc_texture.getSize().y }));
        m_npc_name.setString(npc.get_name());
        sf::FloatRect bounds = m_npc_name.getLocalBounds();
        m_npc_name.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
        m_npc_text.setString("");
    }

    void Dialogue::handle_mouse_pressed_event(const sf::RenderWindow& window, const sf::Event::MouseButtonPressed& event) {
        auto mouse_pos = sf::Vector2f(window.mapPixelToCoords(event.position));
        if (m_end_dialogue.is_hover(mouse_pos)) {
            m_is_visiable = false;
        }
        else if (m_page < m_text.size() - 1 && m_next_page.is_hover(mouse_pos)) {
            m_page++;
            m_npc_text.setString("");
        } 
        else if (m_page > 0 && m_previous_page.is_hover(mouse_pos)) {
            m_page--;
            m_npc_text.setString("");
        }
    }

    void Dialogue::handle_frame(const sf::RenderWindow& window, uint64_t dt) {
        m_timer += dt;
        auto pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        handle_mouse_frame(pos);
        handle_text_frame();
    }

    void Dialogue::handle_mouse_frame(const sf::Vector2f& pos) {
        if (m_end_dialogue.is_hover(sf::Vector2f(pos))) {
            m_end_dialogue.set_text_color(sf::Color::Green);
        }
        else {
            m_end_dialogue.set_text_color(sf::Color::Black);
        }

        if (m_next_page.is_hover(sf::Vector2f(pos))) {
            m_next_page.set_text_color(sf::Color::Green);
        }
        else {
            m_next_page.set_text_color(sf::Color::Black);
        }

        if (m_previous_page.is_hover(sf::Vector2f(pos))) {
            m_previous_page.set_text_color(sf::Color::Green);
        }
        else {
            m_previous_page.set_text_color(sf::Color::Black);
        }
    }

    void Dialogue::handle_text_frame() {
        auto current_text = m_npc_text.getString();
        if (m_timer > 30000 && current_text.getSize() < m_text[m_page].size()) {
            m_timer %= 30000;
            m_npc_text.setString(current_text + m_text[m_page][current_text.getSize()]);
       }
    }

    void Dialogue::draw(sf::RenderWindow& window) {
        window.draw(m_background);
        window.draw(m_npc_background);
        window.draw(*m_npc);
        window.draw(m_npc_name_shape);
        window.draw(m_npc_name);
        window.draw(m_text_background);
        window.draw(m_npc_text);
        m_end_dialogue.draw(window);
        if (m_page > 0) {
            m_previous_page.draw(window);
        }
        if (m_page < m_text.size() - 1) {
            m_next_page.draw(window);
        }
    }
}
