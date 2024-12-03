
#include <iostream>

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600) , "SFML works!");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f));
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition(250, 500);
    
    sf::Texture texture;
    texture.loadFromFile("./Ribbit.png");
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float moveSpeed = 0.5f;
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement.x -= moveSpeed;
            sprite.setScale(-2.f, 2.f);
            sprite.setOrigin(sprite.getLocalBounds().width, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement.x += moveSpeed;
            sprite.setScale(2.f, 2.f);
            sprite.setOrigin(0, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement.y -= moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement.y += moveSpeed;
        }


        sf::Vector2u windowSize = window.getSize();
        sf::FloatRect spriteBounds = sprite.getGlobalBounds();

        sf::Vector2f newPosition = sprite.getPosition() + movement;

        if (newPosition.x < 0) {
            newPosition.x = 0;
        }
        if (newPosition.y < 0) {
            newPosition.y = 0;
        }
        if (newPosition.x + spriteBounds.width > windowSize.x) {
            newPosition.x = windowSize.x - spriteBounds.width;
        }
        if (newPosition.y + spriteBounds.height > windowSize.y) {
            newPosition.y = windowSize.y - spriteBounds.height;
        }

        sprite.setPosition(newPosition);

        window.clear();
        window.draw(shape);
        window.draw(rectangle);
        window.draw(sprite);
        window.display();
    }

    return 0;
}