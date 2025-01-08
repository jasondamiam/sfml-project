#include "DialogueEventManager.h"

DialogueEventManager::DialogueEventManager(sf::Font& font, const sf::Vector2f& boxSize)
{
    dialogueBox.setSize(boxSize);
    dialogueBox.setFillColor(sf::Color(0, 0, 0, 200));
    dialogueBox.setPosition(50.f, 450.f);

    dialogueText.setFont(font);
    dialogueText.setCharacterSize(24);
    dialogueText.setFillColor(sf::Color::White);
    dialogueText.setPosition(dialogueBox.getPosition().x + 10, dialogueBox.getPosition().y + 10);
}

void DialogueEventManager::addEvent(const sf::Vector2f& position, const std::vector<std::string>& dialogue)
{
    events.push_back({ position, dialogue, false });
}

void DialogueEventManager::checkInteraction(const sf::Sprite& sprite, const sf::Keyboard::Key key)
{

    bool enterKeyIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

    if (dialogueActive && enterKeyIsPressed && !enterKeyWasPressed) {
        currentLineIndex++;
        if (currentLineIndex >= (int)events[currentEventIndex].dialogue.size()) {
            dialogueActive = false;
            currentEventIndex = -1;
            currentLineIndex = 0;
        }
        else {
            dialogueText.setString(events[currentEventIndex].dialogue[currentLineIndex]);
        }
    }

    enterKeyWasPressed = enterKeyIsPressed;

    if (sf::Keyboard::isKeyPressed(key) && !dialogueActive) {
        for (int i = 0; i < (int)events.size(); ++i) {
            auto& event = events[i];
            sf::Vector2f playerPos = sprite.getPosition();
            float distance = std::sqrt(std::pow(playerPos.x - event.position.x, 2) + std::pow(playerPos.y - event.position.y, 2));

            if (distance < interactionRadius && !event.triggered) {
                dialogueActive = true;
                currentEventIndex = i;
                currentLineIndex = 0;
                event.triggered = true;
                dialogueText.setString(event.dialogue[currentLineIndex]);
                break;
            }
        }
    }
}

void DialogueEventManager::draw(sf::RenderWindow& window)
{
    if (dialogueActive && currentEventIndex >= 0) {
        window.draw(dialogueBox);
        window.draw(dialogueText);
    }
}
