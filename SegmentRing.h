#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "CircularSector.h"

namespace CClock {
    
    enum class ColorSchema { UNI, ALTERNATING, HALF };
    
    class SegmentRing : public sf::Drawable, public sf::Transformable {
        private:
            std::vector<CircularSector> segments_; 
            std::vector<sf::Text> labels_; 
            
        public:
            explicit SegmentRing();
            explicit SegmentRing(double radius, double thickness, double spacing, unsigned int segmentCount,
                                 unsigned int detailLevel = 30, ColorSchema colorSchema = ColorSchema::UNI,
                                 sf::Color color1 = sf::Color::White, sf::Color color2 = sf::Color::White,
                                 std::string labels[] = nullptr, sf::Font* font = nullptr, unsigned int textSize = 30, sf::Color textColor = sf::Color::Black);
            
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}