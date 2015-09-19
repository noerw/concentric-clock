#include "CircularSector.h"
#include <iostream>//DEBUG

namespace CClock {
    
    CircularSector::CircularSector() {}
    
    CircularSector::CircularSector(const double radius, const double thickness, const double angle, const sf::Color color, const unsigned int segmentCount) {
        
        //init Vector
        vertices_ = sf::VertexArray(sf::TrianglesStrip, 2 + segmentCount * 2);
static int triangleCounter = 0;//DEBUG
        float x, y, tileAngle;
        //for each "segment" add two triangles
        for (unsigned int i = 0; i <= segmentCount; i+=2) {
            //compute angle
            tileAngle = i * (angle / segmentCount) + pi / 2;
            x = std::cos(tileAngle);
            y = std::sin(tileAngle);
            //inner point
            vertices_[i]   = sf::Vertex(sf::Vector2f(x, y) * (float)(radius - thickness), color);
            //outer point
            vertices_[i+1] = sf::Vertex(sf::Vector2f(x, y) * (float)radius, sf::Color(160,160,160) * color);
triangleCounter += 2;//DEBUG
        }
triangleCounter -= 2;//DEBUG
std::cout << triangleCounter << " triangles" << std::endl; //DEBUG
    }

    void CircularSector::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(vertices_, states);
    }
};