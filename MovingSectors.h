#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "CircularSector.h"

namespace CClock {

    class MovingSectors : public sf::Drawable, public sf::Transformable {
        private:
            std::vector<CircularSector> sectors_;
            std::vector<double> radii_;
            std::vector<double> rotSpeeds_;
            double maxSpeed_;
            double angle_ = 2*pi / 5;
            
        public:
            explicit MovingSectors();
            explicit MovingSectors(unsigned int size, double radii[], double rotSpeeds[], sf::Color color = sf::Color::White, double maxSpeed = 200);
            
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            
            void update(double timeStep, sf::RenderWindow& window);
    };
}