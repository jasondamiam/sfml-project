
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class DialogueEventManager
{
public:
	DialogueEventManager(sf::Font& font, const sf::Vector2f& boxSize);
	void addEvent(const sf::Vector2f& position, const std::vector<std::string>& dialogue);
	void checkInteraction(const sf::Sprite& sprite, const sf::Keyboard Key key);
	void draw(sf::RenderWindow& window);
};

