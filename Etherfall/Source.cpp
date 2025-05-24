#include "GameManager/ResourceManager/ResourceManager.h"
#include "GameManager/GameManager.h"
#include <iostream>
#include <Windows.h>

int main()
{
    Etherfall::g_resource_manager = std::make_unique<Etherfall::ResourceManager>(RESOURCE_DIR);
    Etherfall::GameManager game_manager({800, 600});
    //Etherfall::GameManager game_manager;
    game_manager.run();
    //game_manager.run_test();
}

/*
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

struct Platform {
    sf::Vector2f p1, p2;

    Platform(sf::Vector2f start, sf::Vector2f end) : p1(start), p2(end) {}

    void draw(sf::RenderWindow& window) {
        sf::Vertex line[] = {
            sf::Vertex(p1, sf::Color::White),
            sf::Vertex(p2, sf::Color::White)
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // Returns y on the line at a given x
    float getYAtX(float x) const {
        float dx = p2.x - p1.x;
        if (dx == 0) return p1.y;
        float t = (x - p1.x) / dx;
        return p1.y + t * (p2.y - p1.y);
    }

    bool isWithinX(float x) const {
        return (x >= std::min(p1.x, p2.x) && x <= std::max(p1.x, p2.x));
    }
};

struct Player {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool onGround = false;

    Player(sf::Vector2f pos) {
        shape.setSize({ 30, 50 });
        shape.setOrigin({15, 50});  // Origin at bottom center
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::Green);
    }

    void update(float dt, const std::vector<Platform>& platforms) {
        // Horizontal movement first
        float moveX = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            moveX -= 200 * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            moveX += 200 * dt;

        shape.move({ moveX, 0 });

        // Apply gravity
        velocity.y += 500 * dt;
        


        for (const auto& plat : platforms) {
            float playerX = shape.getPosition().x;
            if (plat.isWithinX(playerX)) {
                float groundY = plat.getYAtX(playerX);
                float playerFootY = shape.getPosition().y;

                if (playerFootY >= groundY && playerFootY - velocity.y * dt < groundY) {
                    shape.setPosition({ playerX, groundY });
                    velocity.y = 0;
                    onGround = true;
                }
            }
        }

        shape.move({ 0, velocity.y * dt });

        // Snap to slope height if walking on ground
        if (onGround) {
            for (const auto& plat : platforms) {
                float playerX = shape.getPosition().x;
                if (plat.isWithinX(playerX)) {
                    float groundY = plat.getYAtX(playerX);
                    shape.setPosition({ playerX, groundY });
                    break;
                }
            }
        }

        // Jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround) {
            velocity.y = -300;
            onGround = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Sloped Platform Demo");
    window.setFramerateLimit(60);

    std::vector<Platform> platforms = {
        Platform({ 100, 500 }, { 300, 500 }),   // horizontal
        Platform({ 350, 500 }, { 550, 400 }),   // slope up
        Platform({ 600, 400 }, { 750, 450 })    // slope down
    };

    Player player({ 150, 100 });

    sf::Clock clock;

    while (window.isOpen()) {
        std::optional<sf::Event> ev;
        while (ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        player.update(dt, platforms);

        window.clear(sf::Color::Black);
        for (auto& plat : platforms)
            plat.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}
*/