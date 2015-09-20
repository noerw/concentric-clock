#include <string>
#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ConcentricClock.h"
#include "MovingSectors.h"

namespace CClock {
    
    const std::string title = "Concentric Clock";
    const std::string version = "v0.2";
    const sf::Time updateTimestep = sf::seconds(0.0005);
    const sf::Color bgColor = sf::Color::Black;
    const int antialisingLvl = 4; //set antialising for shapes. doesnt affect textures (-> sf::Texture::setSmooth())
    sf::Vector2f resolution{1280, 800};
    bool fullScreen = false;
    
    const double clockRadius    = 150;
    const double clockRingsize  = 30;
    const double clockSpacing   = 5;
    const double clockThickness = 6 * clockRingsize + 6 * clockSpacing;
    sf::Vector2f zoomSize{90, (float)clockThickness + 45};
    
    // because we want to create the objects in init(), we use the default constructor, so nothing can go wrong here
    sf::RenderWindow window{};
    sf::Font font{};
    sf::View stdView{};
    sf::View zoomedView{};
    
    sf::Text titleText{};
    sf::Text infoText{};
    
    ConcentricClock clock{};
    
    MovingSectors mSectors{};
    
    sf::RectangleShape indicator{};
    sf::RectangleShape zoomRect{};
    sf::RectangleShape darkenRect{};
    sf::CircleShape middleFixCirc{};
    
    void resizeViews(sf::Vector2f size) {
        //change view size (eg on resize event), to display everything porportionally
        stdView.setSize(size);
        stdView.setCenter(0, 0);
        stdView.setViewport(sf::FloatRect(0, 0, 1, 1));
        
        zoomedView.setSize(zoomSize);
        zoomedView.setCenter(0, clockRadius + clockThickness / 2);
        zoomedView.zoom(clockThickness / zoomSize.y + 0.04);
        zoomedView.setViewport(sf::FloatRect(0.5 - (zoomSize.x / size.x) / 2,
                                             0.5 + (clockRadius - ((zoomSize.y - clockThickness) / 2)) / size.y,
                                             zoomSize.x / size.x,
                                             zoomSize.y / size.y));
        //reset darkenRect
        darkenRect.setSize(size);
        darkenRect.setPosition(-size / 2.f);
    }
    
    bool toggleFullscreen() {
        sf::ContextSettings settings;
        settings.antialiasingLevel = antialisingLvl;

        if (!fullScreen) {
            //go fullscreen
            sf::VideoMode fsMode = sf::VideoMode::getFullscreenModes()[0];
            window.create(fsMode, title + " " + version + " [FULLSCREEN]", sf::Style::Fullscreen, settings);
            
            if(!window.isOpen()) {
                std::cout << "Unable to enter fullscreen mode. Aborting execution.";
                return false;
            }

            fullScreen = true;
            resizeViews(sf::Vector2f(fsMode.width, fsMode.height));
        } else {
            //go windowed
            window.create(sf::VideoMode(resolution.x, resolution.y, 32), title + " " + version, sf::Style::None, settings);
            fullScreen = false;
            resizeViews(resolution);
        }

        return true;
    }
    
    bool init() {

        std::string RES_PATH = "./res/";
#ifdef _WIN32
        RES_PATH = "../res/";
#endif

        //load font
        if (!font.loadFromFile(RES_PATH + "Audiowide-Regular.ttf")) {
            std::cout << "Unable to load font. Aborting execution.";
            return false;
        }
        
        //window
        sf::ContextSettings settings;
        settings.antialiasingLevel = antialisingLvl;
        window.create(sf::VideoMode(resolution.x, resolution.y, 32), title + " " + version, sf::Style::None, settings);
        if(!window.isOpen()) {
            std::cout << "Unable to open window object. Aborting execution.";
            return false;
        }

        //views
        resizeViews(resolution);
        
        //texts
        titleText = sf::Text("Concentric Clock", font, 27);
        titleText.setColor(sf::Color::White);
        titleText.setOrigin(titleText.getLocalBounds().width / 2, titleText.getLocalBounds().height / 2);
        titleText.setPosition(0, -22);

        infoText = sf::Text("github.com/noerw", font, 12);
        infoText.setColor(sf::Color(200,200,200));
        infoText.setOrigin(infoText.getLocalBounds().width / 2, infoText.getLocalBounds().height / 2);
        infoText.setPosition(0, 7);
        
        //moving Sectors
        double radii[2] = {clockRadius - 5, clockRadius + clockThickness + 11};
        double rotSpeeds[2] = {40, -20};
        mSectors = MovingSectors{sizeof(radii) / sizeof(*radii), radii, rotSpeeds};

        //half transparent rectShape to darken the clock where its not in the zoomedView
        darkenRect.setSize(resolution);
        darkenRect.setPosition(-resolution / 2.f);
        darkenRect.setFillColor(bgColor * sf::Color(0,0,0,100));
        
        //zoomed indicator
        zoomRect.setSize(zoomSize);
        zoomRect.setOrigin(zoomRect.getLocalBounds().width / 2, zoomRect.getLocalBounds().height / 2);
        zoomRect.setPosition(0, clockRadius + clockThickness / 2);
        zoomRect.setOutlineThickness(4);
        zoomRect.setFillColor(bgColor);
        zoomRect.setOutlineColor(sf::Color::Red * sf::Color(180,180,180,200));     

        indicator.setSize(sf::Vector2f(2, zoomSize.y + 25));
        indicator.setOrigin(indicator.getLocalBounds().width / 2, indicator.getLocalBounds().height / 2);
        indicator.setPosition(0, clockRadius + clockThickness / 2);
        indicator.setFillColor(sf::Color::Red * sf::Color(200,200,200,230));

        //circleshape to hide artifacts in the middle of the clock
        middleFixCirc = sf::CircleShape(clockRadius-1);
        middleFixCirc.setOrigin(middleFixCirc.getLocalBounds().width / 2, middleFixCirc.getLocalBounds().height / 2);
        middleFixCirc.setPosition(0,0);
        middleFixCirc.setFillColor(bgColor);
        
        //the actual clock
        clock = ConcentricClock(clockRadius, clockRingsize, clockSpacing, &font);

        std::cout << "Initialisation complete!\n";
        return true;
    }

    void update(double timeStep) {
        // get & handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            //window
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                case sf::Event::Resized:
                    //resize the views, so everything is still displayed proportionally
                    resizeViews(sf::Vector2f(event.size.width, event.size.height));
                    break;
                    
                case sf::Event::KeyPressed:
                    if      (event.key.code  == sf::Keyboard::Escape)                  window.close();
                    else if (event.key.code  == sf::Keyboard::Q)                       window.close();
                    else if (event.key.code  == sf::Keyboard::F)                       toggleFullscreen();
                    else if (event.key.code  == sf::Keyboard::Return && event.key.alt) toggleFullscreen();
                    break;
                    
                case sf::Event::MouseButtonPressed:
                    toggleFullscreen();
                    break;
                    
                default: break;
            }
        }
        
        //update the content
        clock.update(timeStep);
        mSectors.update(timeStep, window); //window needed for mouseposition
    }

    void draw() {
        window.clear(bgColor);
        
        window.setView(stdView);
        window.draw(clock);
        window.draw(middleFixCirc); //to hide strange artifacts in the middle of the clock
        window.draw(mSectors);
        window.draw(darkenRect);
        window.draw(titleText);
        window.draw(infoText);
        window.draw(zoomRect);
        
        window.setView(zoomedView);
        window.draw(clock);
        
        window.setView(stdView);
        window.draw(indicator);

        window.display();
    }

    void mainloop() {
        sf::Clock clock;
        sf::Time previousTime = sf::milliseconds(0); //time from previous loop
        sf::Time lagTime = sf::milliseconds(0);      //time how many updates we're behind
        
        while(window.isOpen()) {
            //calc elapsed time since last loop
            sf::Time currentTime = clock.getElapsedTime();
            sf::Time elapsedTime = currentTime - previousTime;
            previousTime = currentTime;
            lagTime += elapsedTime; //add elapsed time on time we have to update
            
            //update as many times as we are behind
            while (lagTime >= updateTimestep) {
                update(updateTimestep.asSeconds());
                lagTime -= updateTimestep;
            }
            //draw anytime we can
            draw();
        }
    }

    int run() {
        if(!init()) return 1;
        mainloop();
        return 0;
    }
}


int main() {
    return CClock::run();
}