#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>

#include "SegmentRing.h"

namespace CClock {

    class ConcentricClock : public sf::Drawable, public sf::Transformable {
        private:
            SegmentRing secondRing;
            SegmentRing minuteRing;
            SegmentRing hourRing;
            SegmentRing dayRing;
            SegmentRing monthRing;
            SegmentRing yearRing;
            CircularSector pendulum;
            
            unsigned int baseYear;
            sf::Color color1 = sf::Color::White;
            sf::Color color2 = sf::Color(180, 180, 180);
            unsigned short recalcInterval = 1; //seconds after wich the ringpositions are recalced

            struct tm currentTime;
            double secondsAsDouble = 0;
            
            inline void updateTime();
            void calcRingPositions();

        public:
            explicit ConcentricClock();
            explicit ConcentricClock(double radius, double thickness, double spacing, sf::Font* font);
            
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            
            void update(double timeStep);
    };
}