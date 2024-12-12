#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Barrier.h"
#include "TextureManager.h"

enum MapState {
    start,
    secondmap,
    thirdmap
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "SFML works!");

    // creating the stuff that can be displayed once opened

    sf::Sprite startsprite(TextureManager::getTexture("./MapBackgrounds/start-background.png"));
    sf::Sprite swamp(TextureManager::getTexture("./MapBackgrounds/swamp.png"));
    sf::Sprite sprite(TextureManager::getTexture("./CharacterSprites/Ribbit.png"));

    sprite.setScale(2.f, 2.f);
    sprite.setPosition(300.f, 300.f);

    sf::Music startmapmusic;
    startmapmusic.openFromFile("./newbarktown.ogg");
    startmapmusic.setVolume(100.f);
    startmapmusic.setLoop(true);
    startmapmusic.play();

    std::vector<Barrier> startBarriers = {
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 600.f)), // left barrier
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(440.f, 50.f)), // top left barrier
        Barrier(sf::Vector2f(0.f, 545.f), sf::Vector2f(1000.f, 55.f)), // bottom barrier
        Barrier(sf::Vector2f(950.f, 0.f), sf::Vector2f(10.f, 600.f)), // right barrier
        Barrier(sf::Vector2f(1000.f, 0.f), sf::Vector2f(-470.f, 50.f)) // top right barrier
    };

    std::vector<Barrier> secondMapBarriers = {

    };

    MapState Levels = MapState::start;

    while (window.isOpen())                                 // what you see when you open
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        float moveSpeed = 0.5f;
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {          // movement
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

        sf::Vector2f newPosition = sprite.getPosition() + movement;
        sf::FloatRect newBounds = sprite.getGlobalBounds();
        newBounds.left += movement.x;
        newBounds.top += movement.y;

        const std::vector<Barrier>* barriers = (Levels == MapState::start) ? &startBarriers : &secondMapBarriers;

        for (const Barrier& barrier : *barriers) {
            if (barrier.checkCollision(newBounds)) {
                sf::FloatRect barrierBounds = barrier.getBounds();

                if (newBounds.left < barrierBounds.left && movement.x > 0) {
                    movement.x = 0;
                }
                if (newBounds.left + newBounds.width > barrierBounds.left + barrierBounds.width && movement.x < 0) {
                    movement.x = 0;
                }
                if (newBounds.top < barrierBounds.top && movement.y > 0) {
                    movement.y = 0;
                }
                if (newBounds.top + newBounds.height > barrierBounds.top + barrierBounds.height && movement.y < 0) {
                    movement.y = 0;
                }
            }
        }

        if (Levels == MapState::start) {
            if (sprite.getPosition().y < 0) {
                sprite.setPosition(sprite.getPosition().x, window.getSize().y - 1);
                Levels = MapState::secondmap;
            }
        }
        else if (Levels == MapState::secondmap) {
            if (sprite.getPosition().y > window.getSize().y) {
                sprite.setPosition(sprite.getPosition().x, 1);
                Levels = MapState::start;
            }
            if (sprite.getPosition().y == 521.f) {
                sprite.setPosition(0.f, 500.f);
                Levels = MapState::thirdmap;
            }
        }


        sprite.move(movement);

        window.clear();

        switch (Levels) {                                                   // switch case to make switching maps work
        case MapState::start:
            window.draw(startsprite);
            window.draw(sprite);
            break;
        case MapState::secondmap:
            window.draw(swamp);
            window.draw(sprite);
            break;
        case MapState::thirdmap:
            window.draw(sprite);
            break;
        }

        window.display();                                                    // displays whats on
    }

    return 0;
}