#include <SFML/Graphics.hpp>

namespace Etherfall::SFML {
    class RoundedRectangle : public sf::Drawable, public sf::Transformable {
    public:
        RoundedRectangle(const sf::Vector2f& size,
            const sf::Color& topColor = sf::Color::Black,
            const sf::Color& bottomColor = sf::Color::Black,
            float radius = 10.f,
            unsigned int cornerPoints = 8);

        void setSize(const sf::Vector2f& size);
        void setRadius(float radius);
        void setColors(const sf::Color& topColor, const sf::Color& bottomColor);
        void setCornerPointCount(unsigned int count);
        sf::FloatRect getGlobalBounds() const;

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void updateShape();

    private:
        sf::Vector2f m_size;
        float m_radius;
        unsigned int m_cornerPointCount;
        sf::Color m_topColor;
        sf::Color m_bottomColor;
        sf::VertexArray m_vertices;
    };

}
