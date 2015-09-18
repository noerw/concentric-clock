#pragma once
#include <SFML/Graphics.hpp>

namespace CClock {
    const double pi = 3.141592654;

    class CircularSector : public sf::Drawable, public sf::Transformable {
        private:
            sf::VertexArray vertices_; 
            
        public:
            explicit CircularSector();
            explicit CircularSector(const double radius, const double thickness, const double angleInRad,
                                    const sf::Color color = sf::Color::White, const unsigned int segmentCount = 40);
            
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

};