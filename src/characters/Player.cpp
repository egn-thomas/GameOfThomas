#include "Player.hpp"

// constructeur : on appelle le constructeur de GameCharacter
Player::Player(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture)
    : GameCharacter(name, hp, mana, speed, texture)
{
}

void Player::update() {
    velocity = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        velocity.y = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        velocity.y = speed;
    }

    sprite.move(velocity);
}

void Player::draw(sf::RenderWindow& window) {
    GameCharacter::draw(window); // appelle la méthode parent
}
