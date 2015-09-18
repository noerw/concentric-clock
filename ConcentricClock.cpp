#include "ConcentricClock.h"
#include <string>
#include <iostream>

namespace CClock {
    ConcentricClock::ConcentricClock() {}
    ConcentricClock::ConcentricClock(double radius, double thickness, double spacer, sf::Font* font) {
        updateTime();
        baseYear = currentTime.tm_year + 1900 - 2; //begin year ring 2 years before current year
        
        //init labels
        std::string label60[60];
        std::string label31[31];
        std::string label24[24];
        std::string labelMonth[12] = {"Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
        std::string labelYear[10];
        for (int i = 0; i < 60; ++i) {
            std::string spacer;
            if (i < 10) spacer = "0";
            
            label60[i] = spacer + std::to_string(i);
            if (i < 32 && i > 0) label31[i-1] = spacer + std::to_string(i);
            if (i < 24) label24[i] = spacer + std::to_string(i);
            if (i < 10) labelYear[i] = std::to_string(baseYear + i);
        }
        
        //init rings
        secondRing = SegmentRing(radius +  6* thickness + 6* spacer, thickness, 3, 60,  4, ColorSchema::ALTERNATING, color1, color2, label60, font, 17);
        minuteRing = SegmentRing(radius +  5* thickness + 5* spacer, thickness, 3, 60,  4, ColorSchema::UNI,         color1, color1, label60, font, 19);
        hourRing   = SegmentRing(radius +  4* thickness + 4* spacer, thickness, 3, 24, 12, ColorSchema::HALF,        color1, color2, label24, font, 21);
        dayRing    = SegmentRing(radius +  3* thickness + 2* spacer, thickness, 3, 31,  8, ColorSchema::UNI,         color1, color1, label31, font, 24);
        monthRing  = SegmentRing(radius +  2* thickness + 1* spacer, thickness, 3, 12, 24, ColorSchema::ALTERNATING, color1, color2, labelMonth, font, 24);
        yearRing   = SegmentRing(radius +  1* thickness + 0* spacer, thickness, 3, 10, 24, ColorSchema::UNI,         color1, color1, labelYear, font, 24);
        
        //init time & ringpositions
        calcRingPositions();
        
        //init pendulum
        pendulum = CircularSector(radius + 6* thickness + 6* spacer, 6* thickness + 6* spacer, 20 * pi / 180, color2, 10);
        pendulum.setRotation(-20 / 2);
        
    }
    
    void ConcentricClock::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(pendulum, states);
        target.draw(secondRing, states);
        target.draw(minuteRing, states);
        target.draw(hourRing, states);
        target.draw(dayRing, states);
        target.draw(monthRing, states);
        target.draw(yearRing, states);
    }
    
    void ConcentricClock::update(double timeStep) {
        //recalc Ringpositions every "recalcInterval" seconds
        time_t rawtime;
        time(&rawtime);
        if (localtime(&rawtime)->tm_sec % recalcInterval == 0 &&
            difftime(rawtime, mktime(&currentTime)) > 0) {
            calcRingPositions();
        }
        
        secondRing.rotate(6 * timeStep); // > 6 to make the ring "tick"
        minuteRing.rotate(3.3 * timeStep / 60);
        
        pendulum.setRotation(40* sin(secondsAsDouble * pi) - 20 / 2);
        secondsAsDouble += timeStep;
    }
    
    inline void ConcentricClock::updateTime() {
        time_t rawtime;
        time(&rawtime);
        currentTime = *localtime(&rawtime);
    }
    
    void ConcentricClock::calcRingPositions() {
        updateTime();
        
        double secPercentage   = (float)currentTime.tm_sec / 60;
        double minPercentage   = (float)currentTime.tm_min / 60;
        double hourPercentage  = (float)currentTime.tm_hour / 24;
        double dayPercentage   = ((float)currentTime.tm_mday - 1) / 31;
        double monthPercentage = (float)currentTime.tm_mon / 12;
        double yearPercentage  = ((1900 + (float)currentTime.tm_year) - baseYear) / 10;
        
        secondRing.setRotation(360 * secPercentage);
        minuteRing.setRotation(360 * (minPercentage + secPercentage / 60));
        hourRing  .setRotation(360 * (hourPercentage + minPercentage / 24));
        dayRing   .setRotation(360 * (dayPercentage + hourPercentage / 31));
        monthRing .setRotation(360 * (monthPercentage + dayPercentage / 12 + hourPercentage / 31 / 12));
        yearRing  .setRotation(360 * (yearPercentage + monthPercentage / 10  + dayPercentage / 12 / 10));
        
        if (currentTime.tm_sec % 2 == 0) {
            std::cout << secondsAsDouble << " seconds.\n";
            secondsAsDouble = 0; //to prevent too big numbers and minimize rounding errors
        }
        std::cout << currentTime.tm_sec << " seconds. calculated ringpositions.\n";
    }
}