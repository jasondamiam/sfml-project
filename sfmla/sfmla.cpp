#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Barrier.h"
#include "TextureManager.h"
#include "DialogueEventManager.h"

enum MapState {
    start,
    secondmap,
    thirdmap,
    fourthmap
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "SFML works!");

    // creating the stuff that can be displayed once opened

    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) {
        return -1;
    }

    DialogueEventManager dialogueManager(font, sf::Vector2f(900.f, 100.f));

    sf::Sprite startsprite(TextureManager::getTexture("./MapBackgrounds/start-background.png"));
    sf::Sprite swamp(TextureManager::getTexture("./MapBackgrounds/swamp.png"));
    sf::Sprite castle(TextureManager::getTexture("./MapBackgrounds/Interior-castle.png"));
    sf::Sprite sprite(TextureManager::getTexture("./CharacterSprites/Ribbit.png"));
    sf::Sprite frend(TextureManager::getTexture("./CharacterSprites/Chillkatoe.png"));
    sf::Sprite cultman(TextureManager::getTexture("./CharacterSprites/CultLeader.png"));
    sf::Sprite beelzebub(TextureManager::getTexture("./CharacterSprites/Beelzebub.png"));
    sf::Sprite gun(TextureManager::getTexture("./CharacterSprites/Flintlock.png"));

    sprite.setScale(2.f, 2.f);
    sprite.setPosition(300.f, 300.f);

    frend.setScale(3.f, 3.f);
    frend.setPosition(100.f, 100.f);

    cultman.setScale(3.f, 3.f);
    cultman.setPosition(440.f, 150.f);

    beelzebub.setScale(3.f, 3.f);
    beelzebub.setPosition(440.f, 150.f);

    gun.setScale(2.f, 2.f);
    gun.setPosition(440.f, 350.f);
    

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
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(1000.f, 340.f)), // top barrier
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 600.f)), // left barrier
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(100.f, 600.f)), // right barrier
    };

    std::vector<Barrier> thirdMapBarriers = {
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(1000.f, 1.f)), // top barrier
        Barrier(sf::Vector2f(0.f, 600.f), sf::Vector2f(1000.f, 1.f)), // bottom barrier
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 600.f)), // left barrier
        Barrier(sf::Vector2f(1000.f, 0.f), sf::Vector2f(1.f, 600.f)) // right barrier
    };

    std::vector<Barrier> fourthMapBarriers = {
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(1000.f, 1.f)), // top barrier
        Barrier(sf::Vector2f(0.f, 600.f), sf::Vector2f(1000.f, 1.f)), // bottom barrier
        Barrier(sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 600.f)), // left barrier
        Barrier(sf::Vector2f(1000.f, 0.f), sf::Vector2f(1.f, 600.f)) // right barrier
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

        const std::vector<Barrier>* barriers = nullptr;

        if (Levels == MapState::start) {
            barriers = &startBarriers;
            dialogueManager.addEvent(sf::Vector2f(100.f, 100.f), { "Whats up", "Scary cult up ahead","They wil fuck you in the ass", "I like it I want to go again"}); //Dialogue
        }
        else if (Levels == MapState::secondmap) {
            barriers = &secondMapBarriers;
        }
        else if (Levels == MapState::thirdmap) {
            barriers = &thirdMapBarriers;
            dialogueManager.addEvent(sf::Vector2f(440.f, 150.f), { "Me Is Evil", "Like larrys", "I went to the movies little dids i know...", "It would be a feature...", "A creature feature...", "Featuring...", "The creature :insert shcoked face:"}); //evil cult dialogue
        }
        else if (Levels == MapState::fourthmap) {
            barriers = &fourthMapBarriers;
        }

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

        auto isInsideBox = [](float x, float y, float left, float top, float width, float height) {
            return x >= left && x <= left + width && y >= top && y <= top + height;
            };

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
            if (isInsideBox(sprite.getPosition().x, sprite.getPosition().y, 480.f, 350.f, 20.f, 20.f)) {  // 520.f x 390.f y
                sprite.setPosition(0.f, 500.f);
                Levels = MapState::thirdmap;
            }
        }
        else if (Levels == MapState::thirdmap) {
            if (isInsideBox(sprite.getPosition().x, sprite.getPosition().y, 420.f, 130.f, 40.f, 40.f)) {
                std::cout << "To next map\n";
                sprite.setPosition(100.f, 100.f);
                Levels = MapState::fourthmap;  // Correct assignment
            }
        }

        sprite.move(movement);
        dialogueManager.checkInteraction(sprite, sf::Keyboard::E);          //Interaction Button

        window.clear();

        switch (Levels) {                                                   // switch case to make switching maps work
        case MapState::start:
            window.draw(startsprite);
            window.draw(frend);
            break;
        case MapState::secondmap:
            window.draw(swamp);
            break;
        case MapState::thirdmap:
            window.draw(castle);
            window.draw(cultman);
            break;
        case MapState::fourthmap:
            window.draw(castle);
            window.draw(beelzebub);
            window.draw(gun);
            break;
        }

        window.draw(sprite);
        dialogueManager.draw(window);
        window.display();                                                    // displays whats on
    }

    return 0;
}