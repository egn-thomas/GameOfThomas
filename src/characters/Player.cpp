#include "Player.hpp"

/**
 * @brief Constructeur de la classe Player.
 */
Player::Player(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture)
    : GameCharacter(name, hp, mana, stamina, speed, texture)
{
    sprite.setScale(3.f, 3.f);
}

/**
 * @brief Permet au joueur de sauter s'il est au sol.
 *
 * @param jumpStrength La force du saut.
 */
void Player::jump()
{
    if (isOnGround())
    {
        velocity.y = -this->jumpStrength; // vers le haut, négatif dans SFML
        onGround = false;           // on est maintenant en l'air
    }
}

void Player::attack(Direction dir, std::vector<GameCharacter *> targets)
{
    // prevent attacking while stunned
    if (isStunned)
        return;

    // basic cooldown gating
    if (attackCooldown > 0.f)
        return;

    // require stamina for attacking
    if (!hasStamina(attackStaminaCost))
        return;

    // consume stamina and perform attack
    consumeStamina(attackStaminaCost);

    // perform attack with default attack type
    GameCharacter::attack(dir, targets, AttackType::SwordAttack);

    // allow fast tapping for player — short cooldown
    attackCooldown = playerMinAttackCooldown;
}

/**
 * @brief Dessine le joueur à l'écran.
 *
 * @param window La fenêtre SFML où dessiner le joueur.
 */
void Player::draw(sf::RenderWindow &window)
{
    GameCharacter::draw(window);
}