#include "CharacterFactory.hpp"
#include <iostream>

/**
 * @brief Crée un joueur avec des paramètres prédéfinis.
 *
 * @param windowSize La taille de la fenêtre pour positionner le joueur au centre.
 * @return std::unique_ptr<Player> Pointeur unique vers le joueur créé.
 */
std::unique_ptr<Player> CharacterFactory::createPlayer(const sf::Vector2u &windowSize)
{
    auto textureIdle = std::make_shared<sf::Texture>();
    if (!textureIdle->loadFromFile("../src/assets/images/playerIdleAnimation.png"))
    {
        std::cerr << "Failed to load player texture! Using empty fallback texture." << std::endl;
        // create a small empty texture as a safe fallback so the game can run without assets
        textureIdle->create(32, 32);
    }

    auto textureAttackLeft = std::make_shared<sf::Texture>();
    if (!textureAttackLeft->loadFromFile("../src/assets/images/playerAttackAnimationLeft.png"))
    {
        std::cerr << "Failed to load player attack texture! Using empty fallback texture." << std::endl;
        textureAttackLeft->create(32, 32);
    }

    auto textureAttackRight = std::make_shared<sf::Texture>();
    if (!textureAttackRight->loadFromFile("../src/assets/images/playerAttackAnimationRight.png"))
    {
        std::cerr << "Failed to load player attack texture! Using empty fallback texture." << std::endl;
        textureAttackRight->create(32, 32);
    }

    auto textureWalkLeft = std::make_shared<sf::Texture>();
    if (!textureWalkLeft->loadFromFile("../src/assets/images/playerRunAnimationLeft.png"))
    {
        std::cerr << "Failed to load player walk texture! Using empty fallback texture." << std::endl;
        textureWalkLeft->create(32, 32);
    }

    auto textureWalkRight = std::make_shared<sf::Texture>();
    if (!textureWalkRight->loadFromFile("../src/assets/images/playerRunAnimationRight.png"))
    {
        std::cerr << "Failed to load player walk texture! Using empty fallback texture." << std::endl;
        textureWalkRight->create(32, 32);
    }

    auto player = std::make_unique<Player>("Player", 100, 50, 100, 250.f, textureIdle);

    player->setAnimationTexture(AnimationState::Idle, textureIdle, 2, 32, 32, 4.f);
    player->setAnimationHitbox(AnimationState::Idle, 10.f, 13.f, 12.f, 19.f);

    player->setAnimationTexture(AnimationState::WalkRight, textureWalkRight, 4, 32, 32, 8.f);
    player->setAnimationHitbox(AnimationState::WalkRight, 10.f, 13.f, 12.f, 19.f);

    player->setAnimationTexture(AnimationState::WalkLeft, textureWalkLeft, 4, 32, 32, 8.f);
    player->setAnimationHitbox(AnimationState::WalkLeft, 10.f, 13.f, 12.f, 19.f);
    player->setAnimationSpriteOffset(AnimationState::WalkLeft, 4.f, 0.f);

    player->setAnimationTexture(AnimationState::AttackLeft, textureAttackLeft, 9, 96, 32, 15.f);
    player->setAnimationSpriteOffset(AnimationState::AttackLeft, -58.f, 0.f);

    player->setAnimationTexture(AnimationState::AttackRight, textureAttackRight, 9, 96, 32, 15.f);
    player->setAnimationSpriteOffset(AnimationState::AttackRight, 0.f, 0.f); // no offset needed

    player->setPosition(20.f, 256.f * 8);
    // default hitbox (will be used unless a per-animation hitbox is set)

    player->setHitbox(10.f, 13.f, 12.f, 19.f);
    // Configure attack type params (SwordAttack)
    // range, topOffset, height, damage, delay, knockback, stunDuration

    player->setAttackTypeParams(AttackType::SwordAttack, 180.f, 60.f, 19.f, 10, 0.3f, 1000.f, 0.1f);

    return player;
}

/**
 * @brief Crée une liste de chandelles (ennemis IA) avec des paramètres prédéfinis.
 *
 * @param windowSize La taille de la fenêtre pour positionner les ennemis.
 * @param offset Un décalage à appliquer à la position des ennemis.
 * @return std::vector<std::unique_ptr<NonPlayer>> Vecteur de pointeurs uniques vers les ennemis créés.
 */
std::vector<std::unique_ptr<NonPlayer>> CharacterFactory::createNonPlayer(
    const sf::Vector2u &windowSize,
    const sf::Vector2f &offset)
{
    std::vector<std::unique_ptr<NonPlayer>> npcs;
    auto pnjTexture = std::make_shared<sf::Texture>();
    if (!pnjTexture->loadFromFile("../src/assets/images/nonPlayerBaseAnimation.png"))
    {
        std::cerr << "Failed to load nonPlayer texture! Using empty fallback texture." << std::endl;
        pnjTexture->create(32, 32);
    }

    // Candle 1
    auto candle1 = std::make_unique<Candle>("Candle1", 50, 10, 100, 150.f, pnjTexture);
    candle1->setPosition(windowSize.x / 3.f, windowSize.y / 2.f);
    candle1->setAnimationParams(4, 32, 32, 6.f);
    candle1->setHitbox(3.f, 0.f, 26.f, 32.f);
    // Configure attack parameters for NPC (wide attack range, moderate damage)
    candle1->setAttackTypeParams(AttackType::CandleAttack, 120.f, 20.f, 60.f, 10, 0.f, 1000.f, 0.2f);
    npcs.push_back(std::move(candle1));

    // Candle 2
    auto candle2 = std::make_unique<Candle>("Candle2", 60, 15, 100, 150.f, pnjTexture);
    candle2->setPosition(windowSize.x / 2.f, windowSize.y / 1.8f);
    candle2->setAnimationParams(4, 32, 32, 6.f);
    candle2->setHitbox(3.f, 0.f, 26.f, 32.f);
    // Configure attack parameters for NPC
    candle2->setAttackTypeParams(AttackType::CandleAttack, 120.f, 20.f, 60.f, 10, 0.f, 1000.f, 0.2f);
    npcs.push_back(std::move(candle2));

    return npcs;
}

/**
 * @brief Crée une Candle à une position spécifique
 * @param position Position du mob dans le monde
 * @return std::unique_ptr<Candle> Pointeur unique vers la Candle créée
 */
std::unique_ptr<Candle> CharacterFactory::createCandle(const sf::Vector2f &position)
{
    auto pnjTexture = std::make_shared<sf::Texture>();
    if (!pnjTexture->loadFromFile("../src/assets/images/nonPlayerBaseAnimation.png"))
    {
        std::cerr << "Failed to load nonPlayer texture! Using empty fallback texture." << std::endl;
        pnjTexture->create(32, 32);
    }

    auto candle = std::make_unique<Candle>("Candle", 50, 10, 100, 150.f, pnjTexture);
    candle->setPosition(position.x, position.y);
    candle->setAnimationParams(4, 32, 32, 6.f);
    candle->setHitbox(3.f, 0.f, 26.f, 32.f);
    candle->setAttackTypeParams(AttackType::CandleAttack, 120.f, 20.f, 60.f, 10, 0.f, 1000.f, 0.2f);

    return candle;
}
