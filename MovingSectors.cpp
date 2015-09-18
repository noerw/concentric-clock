#include "MovingSectors.h"

namespace CClock {

    MovingSectors::MovingSectors() {}
    MovingSectors::MovingSectors(unsigned int size, double radii[], double rotSpeeds[], sf::Color color, double maxSpeed) {
        sectors_.resize(size);
        radii_.resize(size);
        rotSpeeds_.resize(size);
        
        for (unsigned int i = 0; i < radii_.size(); ++i) {
            radii_[i] = radii[i];
            rotSpeeds_[i] = rotSpeeds[i];
        }
        maxSpeed_ = maxSpeed;
        
        for (unsigned int i = 0; i < radii_.size(); ++i) {
            sectors_[i] = CircularSector(radii_[i], 6, angle_, color);
        }
    }
    
    void MovingSectors::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for (auto& i : sectors_) {
            target.draw(i, states);
        }
    }
    
    void MovingSectors::update(double timeStep, sf::RenderWindow& window) {
        //rotate all sectors with predefined speeds
        sf::Vector2i mousePos          = sf::Mouse::getPosition(window);
        sf::Vector2u windowSize      = window.getSize();
        sf::Vector2f mouseToCenter = sf::Vector2f((float)mousePos.x - ((float)windowSize.x / 2), (float)mousePos.y - ((float)windowSize.y / 2));
        double mouseToCenterLength = std::sqrt((mouseToCenter.x * mouseToCenter.x) + (mouseToCenter.y * mouseToCenter.y));
        for (unsigned int i = 0; i < sectors_.size(); ++i) {
            if (mouseToCenterLength > radii_[i]) {
                sectors_[i].rotate(rotSpeeds_[i] * timeStep);
            }
            //if mouse is within radius, rotate sector with maxspeed to mousepos
            else {
                float rotation = sectors_[i].getRotation();
                float targetRotation = -180 - (atan2(mouseToCenter.x , mouseToCenter.y) + angle_ / 2) * 180 / pi;
                float rotationDiff = std::remainder(targetRotation - rotation + 180, 360);
                //limit rotationspeed
                if (rotationDiff > maxSpeed_ * timeStep) {
                    rotation -= maxSpeed_ * timeStep;
                }
                else if (rotationDiff < -maxSpeed_ * timeStep) {
                    rotation += maxSpeed_ * timeStep;
                }
                sectors_[i].setRotation(rotation);
            }
        }
    }
}