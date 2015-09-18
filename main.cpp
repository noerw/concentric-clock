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
    sf::Vector2f resolution{1000, 800};
    bool fullScreen = false;
    
    double clockRadius    = 150;
    double clockRingsize  = 30;
    double clockSpacing   = 5;
    double clockThickness = 6* clockRingsize + 6* clockSpacing;
    sf::Vector2f zoomSize{80, (float)clockThickness + 45};    
    
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
    
    void resizeViews() {
        //change view size (eg on resize event), to display everything porportionally
        stdView.setSize(resolution);
        stdView.setCenter(0, 0);
        stdView.setViewport(sf::FloatRect(0, 0, 1, 1));
        
        zoomedView.setSize(zoomSize);
        zoomedView.setCenter(0, clockRadius + clockThickness / 2);
        zoomedView.zoom(clockThickness / zoomSize.y + 0.04);
        zoomedView.setViewport(sf::FloatRect(0.5 - (zoomSize.x / resolution.x) / 2,
                                             0.5 + (clockRadius - ((zoomSize.y - clockThickness) / 2)) / resolution.y,
                                             zoomSize.x / resolution.x,
                                             zoomSize.y / resolution.y));
        //reset darkenRect
        darkenRect.setSize(resolution);
        darkenRect.setPosition(-resolution / 2.f);
    }
    
    bool toggleFullscreen() {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 4;
        if (!fullScreen) {
            //go fullscreen
            sf::VideoMode fsMode = sf::VideoMode::getFullscreenModes()[0];
            resolution = sf::Vector2f(fsMode.width, fsMode.height);
            window.create(fsMode, title + " " + version + " [FULLSCREEN]", sf::Style::Fullscreen, settings);
            if(!window.isOpen()) {
                std::cout << "Unable to enter fullscreen mode. Aborting execution.";
                return false;
            }
            fullScreen = true;
        } else {
            //go windowed
            resolution = sf::Vector2f(1000, 800);
            window.create(sf::VideoMode(resolution.x, resolution.y, 32), title + " " + version, sf::Style::None, settings);
            window.setPosition(sf::Vector2i(10,10));
            fullScreen = false;
        }
        resizeViews(); //reset resolution & resize views, because no resize event is fired
        return true;
    }
    
    bool init() {
        //load font
        if (!font.loadFromFile("Audiowide-Regular.ttf")) {
            std::cout << "Unable to load font. Aborting execution.";
            return false;
        }
        
        //window
        sf::ContextSettings settings;
        settings.antialiasingLevel = 4; //set antialising for shapes. doesnt affect textures (-> sf::Texture::setSmooth())
        window.create(sf::VideoMode(resolution.x, resolution.y, 32), title + " " + version, sf::Style::None, settings);
        window.setPosition(sf::Vector2i(10,10));
        if(!window.isOpen()) {
            std::cout << "Unable to open window object. Aborting execution.";
            return false;
        }

        //views
        resizeViews();
        
        //texts
        titleText = sf::Text("Concentric Clock", font, 27);
        titleText.setColor(sf::Color::White);
        titleText.setOrigin(titleText.getLocalBounds().width / 2, titleText.getLocalBounds().height / 2);
        titleText.setPosition(0, -40);

        infoText = sf::Text("http://github.com/noerw", font, 12);
        infoText.setColor(sf::Color(200,200,200));
        infoText.setOrigin(infoText.getLocalBounds().width / 2, infoText.getLocalBounds().height / 2);
        infoText.setPosition(0, -10);
        
        //moving Sectors
        double radii[2] = {clockRadius - 5, clockRadius + clockThickness + 11};
        double rotSpeeds[2] = { 40, -20};
        mSectors = MovingSectors{sizeof(radii)/sizeof(*radii), radii, rotSpeeds};

        //half transparent rectShape to darken the clock where its not in the zoomedView
        darkenRect.setSize(resolution);
        darkenRect.setPosition(-resolution / 2.f);
        darkenRect.setFillColor(sf::Color(0,0,5,90));
        
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
                    resolution = sf::Vector2f(event.size.width, event.size.height);
                    //resize the views, so everything is still displayed proportionally
                    resizeViews();
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