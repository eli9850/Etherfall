#include "RoundedRectangle.h"

constexpr float pi = 3.14159265f;

namespace Etherfall::SFML {

    RoundedRectangle::RoundedRectangle(const sf::Vector2f& size, const sf::Color& topColor, const sf::Color& bottomColor, float radius, unsigned int cornerPoints)
        : m_size(size), m_radius(radius), m_cornerPointCount(cornerPoints),
        m_topColor(topColor), m_bottomColor(bottomColor),
        m_vertices(sf::PrimitiveType::Triangles)
    {
        updateShape();
    }

    void RoundedRectangle::setSize(const sf::Vector2f& size) {
        m_size = size;
        updateShape();
    }

    void RoundedRectangle::setRadius(float radius) {
        m_radius = radius;
        updateShape();
    }

    void RoundedRectangle::setColors(const sf::Color& topColor, const sf::Color& bottomColor) {
        m_topColor = topColor;
        m_bottomColor = bottomColor;
        updateShape();
    }

    void RoundedRectangle::setCornerPointCount(unsigned int count) {
        m_cornerPointCount = count;
        updateShape();
    }

    void RoundedRectangle::updateShape() {
        m_vertices.clear();

        auto interpolateColor = [&](float y) {
            float t = y / m_size.y;
            uint8_t r = static_cast<uint8_t>(m_topColor.r * (1 - t) + m_bottomColor.r * t);
            uint8_t g = static_cast<uint8_t>(m_topColor.g * (1 - t) + m_bottomColor.g * t);
            uint8_t b = static_cast<uint8_t>(m_topColor.b * (1 - t) + m_bottomColor.b * t);
            uint8_t a = static_cast<uint8_t>(m_topColor.a * (1 - t) + m_bottomColor.a * t);
            return sf::Color(r, g, b, a);
        };

        auto addTriangle = [&](sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
            m_vertices.append({ p1, interpolateColor(p1.y) });
            m_vertices.append({ p2, interpolateColor(p2.y) });
            m_vertices.append({ p3, interpolateColor(p3.y) });
        };

        sf::FloatRect inner = {
            {m_radius, m_radius},
            {m_size.x - 2 * m_radius,
            m_size.y - 2 * m_radius}
        };

        float left = inner.position.x, top = inner.position.y, width = inner.size.x, height = inner.size.y;
        addTriangle({ left, top }, { left + width, top }, { left + width, top + height });
        addTriangle({ left, top }, { left + width, top + height }, { left, top + height });
        addTriangle({ m_radius, 0 }, { m_size.x - m_radius, 0 }, { m_size.x - m_radius, m_radius });
        addTriangle({ m_radius, 0 }, { m_size.x - m_radius, m_radius }, { m_radius, m_radius });
        addTriangle({ m_radius, m_size.y - m_radius }, { m_size.x - m_radius, m_size.y - m_radius }, { m_size.x - m_radius, m_size.y });
        addTriangle({ m_radius, m_size.y - m_radius }, { m_size.x - m_radius, m_size.y }, { m_radius, m_size.y });
        addTriangle({ 0, m_radius }, { m_radius, m_radius }, { m_radius, m_size.y - m_radius });
        addTriangle({ 0, m_radius }, { m_radius, m_size.y - m_radius }, { 0, m_size.y - m_radius });
        addTriangle({ m_size.x - m_radius, m_radius }, { m_size.x, m_radius }, { m_size.x, m_size.y - m_radius });
        addTriangle({ m_size.x - m_radius, m_radius }, { m_size.x, m_size.y - m_radius }, { m_size.x - m_radius, m_size.y - m_radius });

        auto addCorner = [&](sf::Vector2f center, float startAngle, float endAngle) {
            sf::Vector2f origin = center;
            for (unsigned int i = 0; i < m_cornerPointCount - 1; ++i) {
                float angle1 = startAngle + (endAngle - startAngle) * i / (m_cornerPointCount - 1);
                float angle2 = startAngle + (endAngle - startAngle) * (i + 1) / (m_cornerPointCount - 1);

                auto point1 = sf::Vector2f(std::cos(angle1), std::sin(angle1)) * m_radius + center;
                auto point2 = sf::Vector2f(std::cos(angle2), std::sin(angle2)) * m_radius + center;

                addTriangle(center, point1, point2);
            }
        };

        addCorner({ m_radius, m_radius }, pi, 1.5f * pi);
        addCorner({ m_size.x - m_radius, m_radius }, 1.5f * pi, 2 * pi);
        addCorner({ m_size.x - m_radius, m_size.y - m_radius }, 0, 0.5f * pi);
        addCorner({ m_radius, m_size.y - m_radius }, 0.5f * pi, pi);
    }

    sf::FloatRect RoundedRectangle::getGlobalBounds() const {
        return getTransform().transformRect({ {0.f, 0.f}, m_size });
    }

    void RoundedRectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform(); // Apply transform
        target.draw(m_vertices, states);
    }

}
