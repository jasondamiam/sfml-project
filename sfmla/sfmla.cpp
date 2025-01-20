#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include "Barrier.h"
#include "TextureManager.h"
#include "MapState.h"
#include "DialogueEventManager.h"

sf::Vector2f getRandomPosition(float minX, float maxX, float minY, float maxY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(minX, maxX);
    std::uniform_real_distribution<float> distY(minY, maxY);
    return sf::Vector2f(distX(gen), distY(gen));
}

enum GameState {
    Running,
    Ending1,
    Ending2
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 600), "SFML works!");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) {
        return -1;
    }

    DialogueEventManager dialogueManager(font, sf::Vector2f(900.f, 100.f));

    // Load textures and sprites
    sf::Sprite startsprite(TextureManager::getTexture("./MapBackgrounds/start-background.png"));
    sf::Sprite swamp(TextureManager::getTexture("./MapBackgrounds/swamp.png"));
    sf::Sprite castle(TextureManager::getTexture("./MapBackgrounds/Interior-castle.png"));
    sf::Sprite sprite(TextureManager::getTexture("./CharacterSprites/Ribbit.png"));
    sf::Sprite frend(TextureManager::getTexture("./CharacterSprites/Chillkatoe.png"));
    sf::Sprite cultman(TextureManager::getTexture("./CharacterSprites/CultLeader.png"));
    sf::Sprite beelzebub(TextureManager::getTexture("./CharacterSprites/Beelzebub.png"));
    sf::Sprite gun(TextureManager::getTexture("./CharacterSprites/Flintlock.png"));
    sf::Sprite ending1(TextureManager::getTexture("./Endings/Ending1.png"));
    sf::Sprite ending2(TextureManager::getTexture("./Endings/Ending2.png"));

    // Set up sprites
    sprite.setScale(2.f, 2.f);
    sprite.setPosition(300.f, 300.f);

    frend.setScale(3.f, 3.f);
    frend.setPosition(100.f, 100.f);

    cultman.setScale(3.f, 3.f);
    cultman.setPosition(440.f, 150.f);

    beelzebub.setScale(3.f, 3.f);
    beelzebub.setPosition(440.f, 150.f);

    gun.setScale(2.f, 2.f);
    gun.setPosition(getRandomPosition(0.f, 1000.f - gun.getGlobalBounds().width, 0.f, 600.f - gun.getGlobalBounds().height));

    sf::Music startmapmusic;
    sf::Music finalmusic;

    startmapmusic.openFromFile("./newbarktown.ogg");
    startmapmusic.setVolume(100.f);
    startmapmusic.setLoop(true);
    startmapmusic.play();

    finalmusic.openFromFile("./cynthia.ogg");
    finalmusic.setVolume(100.f);
    finalmusic.setLoop(true);

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
        Barrier(sf::Vector2f(0.f, 400.f), sf::Vector2f(460.f, 200.f)), // bottom left barrier
        Barrier(sf::Vector2f(1000.f, 400.f), sf::Vector2f(-430.f, 200.f)) // bottom right barrier
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
    GameState gameState = GameState::Running;
    bool finalMusicStarted = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameState == GameState::Running) {
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

            sf::Vector2f newPosition = sprite.getPosition() + movement;
            sf::FloatRect newBounds = sprite.getGlobalBounds();
            newBounds.left += movement.x;
            newBounds.top += movement.y;

            const std::vector<Barrier>* barriers = nullptr;

            if (Levels == MapState::start) {
                barriers = &startBarriers;
                dialogueManager.addEvent(sf::Vector2f(100.f, 100.f), { "Hello traveler!", "I see your finally awake", "There is a scary cult upahead", "Decapatate their leader" }, MapState::start); //Dialogue
            }
            else if (Levels == MapState::secondmap) {
                barriers = &secondMapBarriers;
            }
            else if (Levels == MapState::thirdmap) {
                barriers = &thirdMapBarriers;
                dialogueManager.addEvent(sf::Vector2f(440.f, 150.f), { "Greetings Inferior", "It seemeth you are a frog", "As you are a frog you are my natural enemy", "After all...", "I am", "Beelzebub", "Lord of the flies, Sin of Gluttony and Envy", "THE GUN!", "I will take it first!!"}, MapState::thirdmap, MapState::fourthmap); //evil cult dialogue
            }
            else if (Levels == MapState::fourthmap) {
                barriers = &fourthMapBarriers;

                if (!finalMusicStarted) {
                    startmapmusic.stop();
                    finalmusic.play();
                    finalMusicStarted = true; // Ensure the music only starts once
                }
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
                    sprite.setPosition(500.f, 500.f);
                    Levels = MapState::thirdmap;
                }
            }

            // Beelzebub moves to gun
            if (Levels == MapState::fourthmap) {
                sf::Vector2f direction = gun.getPosition() - beelzebub.getPosition();
                float length = std::hypot(direction.x, direction.y);
                if (length != 0) {
                    direction /= length;
                }
                float speed = 0.3f;
                beelzebub.move(direction * speed);
            }

            sprite.move(movement);
            MapState newMapState = dialogueManager.checkInteraction(sprite, sf::Keyboard::E, Levels);
            if (newMapState != Levels) {
                Levels = newMapState; 
            }

            if (Levels == MapState::fourthmap) {
                if (sprite.getGlobalBounds().intersects(gun.getGlobalBounds())) {
                    gameState = GameState::Ending1;
                    startmapmusic.stop();
                }
                if (beelzebub.getGlobalBounds().intersects(gun.getGlobalBounds())) {
                    gameState = GameState::Ending2;
                }
            }
        }

        window.clear();

        switch (Levels) {
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

        // Display endings
        if (gameState == GameState::Ending1) {
            sf::Text endingText("Ending 1: You Win", font, 50);
            endingText.setPosition(100.f, 100.f);
            window.draw(ending1);
            window.draw(endingText);
        }

        if (gameState == GameState::Ending2) {
            sf::Text endingText("Ending 2: Beelzebub Win's", font, 50);
            endingText.setPosition(100.f, 100.f);
            window.draw(ending2);
            window.draw(endingText);
        }

        window.display();
    }

    return 0;
}