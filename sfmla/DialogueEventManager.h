
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class DialogueEventManager
{
public:
	DialogueEventManager(sf::Font& font, const sf::Vector2f& boxSize);
	void addEvent(const sf::Vector2f& position, const std::vector<std::string>& dialogue);
	void draw(sf::RenderWindow& window);
};

