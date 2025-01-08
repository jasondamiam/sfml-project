
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class DialogueEventManager
{
public:
	DialogueEventManager(sf::Font& font, const sf::Vector2f& boxSize);
	void addEvent(const sf::Vector2f& position, const std::vector<std::string>& dialogue);
	void checkInteraction(const sf::Sprite& sprite, const sf::Keyboard::Key key);
	void draw(sf::RenderWindow& window);
	bool isDialogueComplete() const;

private:
	struct DialogueEvent {
		sf::Vector2f position;
		std::vector<std::string> dialogue;
		bool triggered = false;
	};

	sf::RectangleShape dialogueBox;
	sf::Text dialogueText;
	std::vector<DialogueEvent> events;

	bool enterKeyWasPressed = false;

	int currentEventIndex = -1;
	int currentLineIndex = 0;
	bool dialogueActive = false;
	bool dialogueComplete = false;

	static constexpr float interactionRadius = 50.f;
};

