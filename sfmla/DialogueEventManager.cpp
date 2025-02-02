#include "DialogueEventManager.h"

DialogueEventManager::DialogueEventManager(sf::Font& font, const sf::Vector2f& boxSize) // dialoguebox design
{
    dialogueBox.setSize(boxSize);
    dialogueBox.setFillColor(sf::Color(0, 0, 0, 200));
    dialogueBox.setPosition(50.f, 450.f);

    dialogueText.setFont(font);
    dialogueText.setCharacterSize(24);
    dialogueText.setFillColor(sf::Color::White);
    dialogueText.setPosition(dialogueBox.getPosition().x + 10, dialogueBox.getPosition().y + 10);
}

void DialogueEventManager::addEvent(const sf::Vector2f& position, const std::vector<std::string>& dialogue, MapState mapState, MapState targetMapState) // where the dialogue is placed
{
    events.push_back({ position, dialogue, false, mapState, targetMapState });
}

MapState DialogueEventManager::checkInteraction(const sf::Sprite& sprite, const sf::Keyboard::Key key, MapState currentMapState)
{
    bool enterKeyIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

    // Check if the Enter key was just pressed (not held)
    if (dialogueActive && enterKeyIsPressed && !enterKeyWasPressed) {
        currentLineIndex++;
        if (currentLineIndex >= (int)events[currentEventIndex].dialogue.size()) { 
            dialogueActive = false;
            MapState targetMapState = events[currentEventIndex].targetMapState; 
            currentEventIndex = -1; 
            currentLineIndex = 0;
            dialogueComplete = true;
            return targetMapState; 
        }
        else {
            dialogueText.setString(events[currentEventIndex].dialogue[currentLineIndex]);
        }
    }

    enterKeyWasPressed = enterKeyIsPressed;

    // Handle interaction with events
    if (sf::Keyboard::isKeyPressed(key) && !dialogueActive) {
        for (int i = 0; i < (int)events.size(); ++i) {
            auto& event = events[i];
            sf::Vector2f playerPos = sprite.getPosition();
            float distance = std::sqrt(std::pow(playerPos.x - event.position.x, 2) + std::pow(playerPos.y - event.position.y, 2));

            if (distance < interactionRadius && !event.triggered && event.mapState == currentMapState) {
                dialogueActive = true;
                currentEventIndex = i;
                currentLineIndex = 0;
                event.triggered = true;
                dialogueText.setString(event.dialogue[currentLineIndex]);
                dialogueComplete = false; 
                break;
            }
        }
    }

    return currentMapState; 
}

void DialogueEventManager::draw(sf::RenderWindow& window)
{
    if (dialogueActive && currentEventIndex >= 0) {
        window.draw(dialogueBox);
        window.draw(dialogueText);
    }
}

bool DialogueEventManager::isDialogueComplete() const
{
    return dialogueComplete;
}
