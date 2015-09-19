#include "SegmentRing.h"

namespace CClock {

    SegmentRing::SegmentRing() {}
    
    SegmentRing::SegmentRing(double radius, double thickness, double spacer, unsigned int segmentCount,
                             unsigned int detailLevel, ColorSchema colorSchema, sf::Color color1, sf::Color color2,
                             std::string labels[], sf::Font* font, unsigned int textSize, sf::Color textColor) {
        
        //init circle segments
        segments_.resize(segmentCount);
        double anglePerSegment = 2 * pi / segmentCount;
        double spacerAngle = std::asin(spacer / (radius - thickness));
        double segmentAngle = anglePerSegment - spacerAngle; //minus the spacer
        
        switch (colorSchema) {
            case ColorSchema::UNI:
                color2 = color1;
                
            case ColorSchema::ALTERNATING:
                for (unsigned int i = 0; i < segmentCount; i+=2) {
                    segments_[i] = CircularSector(radius, thickness, segmentAngle, color1, detailLevel);
                    segments_[i].setRotation(i * anglePerSegment * 180 / pi);
                }
                for (unsigned int i = 1; i < segmentCount; i+=2) {
                    segments_[i] = CircularSector(radius, thickness, segmentAngle, color2, detailLevel);
                    segments_[i].setRotation(i * anglePerSegment * 180 / pi);
                }
                break;
                
            case ColorSchema::HALF:
                for (unsigned int i = 0; i < (unsigned int)segmentCount/2; ++i) {
                    segments_[i] = CircularSector(radius, thickness, segmentAngle, color1, detailLevel);
                    segments_[i].setRotation(i * anglePerSegment * 180 / pi);
                }
                for (unsigned int i = (unsigned int)segmentCount/2; i < segmentCount; ++i) {
                    segments_[i] = CircularSector(radius, thickness, segmentAngle, color2, detailLevel);
                    segments_[i].setRotation(i * anglePerSegment * 180 / pi);
                }
                break;
        }
        
        //init labels
        if(labels != nullptr) {
            labels_.resize(segmentCount);
            for (unsigned int i = 0; i < segmentCount; ++i) {
                labels_[i] = sf::Text(labels[i], *font, textSize);
                labels_[i].setColor(textColor);
                sf::FloatRect labelBounds = labels_[i].getLocalBounds();
                labels_[i].setOrigin(labelBounds.width / 2, thickness/1.1 - radius + labelBounds.height/10);
                labels_[i].setRotation((i * -anglePerSegment - anglePerSegment / 2 - spacerAngle / 2)  * 180 / pi);
            }
        }
    }
    
    void SegmentRing::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto& i: segments_) {
            target.draw(i, states);
        }
        if (labels_.size() > 0) {
            for(auto& i: labels_) {
                target.draw(i, states);
            }
        }
    }
}