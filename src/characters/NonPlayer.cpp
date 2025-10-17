#include "NonPlayer.hpp"

// constructeur : on appelle le constructeur de GameCharacter
NonPlayer::NonPlayer(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture)
    : GameCharacter(name, hp, mana, speed, texture)
{
}

void NonPlayer::draw(sf::RenderWindow& window) {
    GameCharacter::draw(window); // appelle la méthode parent
}
