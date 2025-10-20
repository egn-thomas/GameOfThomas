#include "NonPlayer.hpp"

/**
 * @brief Appelle le constructeur de la classe de base GameCharacter.
 */
NonPlayer::NonPlayer(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture)
    : GameCharacter(name, hp, mana, speed, texture)
{
    sprite.setScale(4.f, 4.f);
}

/**
 * @brief Dessine le PNJ en appelant la méthode de la classe de base.
 */
void NonPlayer::draw(sf::RenderWindow &window)
{
    GameCharacter::draw(window);
}
