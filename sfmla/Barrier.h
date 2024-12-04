#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Barrier
{
private:
	sf::FloatRect barrierBounds;
public:
	Barrier(sf::Vector2f position, sf::Vector2f size);

	bool checkCollision(const sf::FloatRect& spriteBounds) const;

	const sf::FloatRect& getBounds() const;
};

