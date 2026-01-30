#include "NonPlayer.hpp"

/**
 * @brief Appelle le constructeur de la classe de base GameCharacter.
 */
NonPlayer::NonPlayer(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture)
    : GameCharacter(name, hp, mana, stamina, speed, texture)
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

/**
 * @brief Comportement IA par défaut (à surcharger dans les classes dérivées)
 * Par défaut, le PNJ ne fait rien.
 */
void NonPlayer::updateBehavior(float deltaTime, Player* player, const std::vector<std::unique_ptr<Ground>>& grounds)
{
    // Implémentation par défaut : ne rien faire
    // Les classes dérivées peuvent surcharger cette méthode
}
