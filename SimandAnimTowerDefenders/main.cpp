#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "SFML works!");
    sf::CircleShape shape(100.f);
    sf::Texture mapTexture; 
    std::cout << mapTexture.loadFromFile("../textures/map1.png");
    sf::Sprite mapSprite(mapTexture);
    sf::Vector2u textureSize = mapTexture.getSize();
    mapSprite.scale(2,2);
    mapSprite.setOrigin(textureSize.x/2, textureSize.y/2);
    mapSprite.setPosition(512, 512);

    shape.setTexture(&mapTexture);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    

  
    shape.setFillColor(sf::Color::Green);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                shape.setPosition(static_cast<sf::Vector2f>(mousePos));


                shape.setFillColor(sf::Color::Red);
            }
        }

        window.clear();
        window.draw(mapSprite);

        window.draw(shape);
        window.display();
    }

    return 0;
}