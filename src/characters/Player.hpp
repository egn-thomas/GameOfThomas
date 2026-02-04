#pragma once
#include "GameCharacter.hpp"
#include <memory>

class Player : public GameCharacter {
protected:
    const float jumpStrength = 520.f;
    // Player-specific minimum attack cooldown (allow fast tapping but cost stamina)
    // float playerMinAttackCooldown = 0.05f;
    // Attack cooldown to prevent spam
    const float attackCooldownDuration = 0.25f;
public:
    // constructeur
    Player(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture);

    void jump();

    // Player-specific attack override to consume stamina and allow fast attacks
    void attack(Direction dir, std::vector<GameCharacter *> targets, const std::vector<std::unique_ptr<Ground>> &grounds) override;

    // dessin éventuellement surchargé (si spécifique)
    void draw(sf::RenderWindow& window) override;
};
