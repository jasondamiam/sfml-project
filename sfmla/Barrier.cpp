#include "Barrier.h"

Barrier::Barrier(sf::Vector2f position, sf::Vector2f size)
{
    barrierBounds = sf::FloatRect(position, size);
}

bool Barrier::checkCollision(const sf::FloatRect& spriteBounds) const
{
    return barrierBounds.intersects(spriteBounds);
}

const sf::FloatRect& Barrier::getBounds() const
{
    return barrierBounds;
}
