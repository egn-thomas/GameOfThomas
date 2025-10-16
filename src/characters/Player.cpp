#include "Player.hpp"

// constructeur : on appelle le constructeur de GameCharacter
Player::Player(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture)
    : GameCharacter(name, hp, mana, speed, texture)
{
}

void Player::jump(float jumpStrength) {
    if (isOnGround()) {
        velocity.y = -jumpStrength; // vers le haut, négatif dans SFML
        onGround = false;           // on est maintenant en l'air
    }
}

void Player::draw(sf::RenderWindow& window) {
    GameCharacter::draw(window); // appelle la méthode parent
}
