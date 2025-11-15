#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../environnement/Ground.hpp"
#include "Direction.hpp"
#include <memory>
#include <unordered_map>

enum class AnimationState
{
    Idle,
    Walk
};

class GameCharacter
{
private:
    std::string name;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int level;
    float speed;

    sf::Vector2f position;
    sf::Vector2f previousPosition;

    // Pour gérer l'animation :

    AnimationState currentState = AnimationState::Idle;
    float stillTimer = 0.f;

    int frameCount;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    float frameTime;
    float timer;

    float gravity = 1500.f;

    bool contactTop;
    bool contactBottom;
    bool contactLeft;
    bool contactRight;

    // Direction du personnage
    bool facingLeft = false;

    // --- Dash ---
    bool isDashing = false;
    bool canDash = true;
    float dashSpeed = 1200.f;
    float dashDuration = 0.15f;
    float dashTimer = 0.f;
    int dashDirection = 0; // -1 = gauche, +1 = droite

    // Attack

    float attackCooldown;                 // Temps restant avant la prochaine attaque
    const float attackCooldownMax = 0.5f; // Durée du cooldown en secondes

    bool isDamaged = false;
    float damageTimer = 0.f;

protected:
    std::shared_ptr<sf::Texture> texture;
    bool onGround = false;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    sf::FloatRect hitbox;

    struct AnimationData
    {
        std::shared_ptr<sf::Texture> texture;
        int frameCount;
        int frameWidth;
        int frameHeight;
        float frameTime;
    };

    std::unordered_map<AnimationState, AnimationData> animations;

public:
    GameCharacter(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);
    virtual ~GameCharacter() = default;

    // Méthodes essentielles

    void update(float deltaTime, const std::vector<std::unique_ptr<Ground>> &grounds);

    // Position et mouvement

    void applyAllForces(float deltaTime);
    void applyGravity(float deltaTime);
    void move(const sf::Vector2f &offset);
    void startDash(int direction);
    void applyCollisions();
    void checkAllCollisions(const std::vector<std::unique_ptr<Ground>> &grounds);
    void checkCollisionWithGround(const Ground &ground);
    void collisionsToZero();
    void setHitbox(float offsetX, float offsetY, float width, float height);
    virtual sf::FloatRect getBounds() const;
    bool isOnGround() const { return onGround; }
    
    // Visuel et animation
    
    void selfAnimator(float deltaTime);
    void walkAnimator(float deltaTime);
    void setAnimationTexture(AnimationState state, std::shared_ptr<sf::Texture> texture, int frameCount, int frameWidth, int frameHeight, float fps);
    void setAnimationState(AnimationState newState);
    virtual void draw(sf::RenderWindow &window);
    void setAnimationParams(int frameCount, int frameWidth, int frameHeight, float fps);
    
    // Combat
    
    void attack(Direction dir, std::vector<GameCharacter *> targets);
    
    // Gestion des stats
    
    void takeDamage(int dmg);
    bool isAlive() const;
    void allCooldowns(float deltaTime);
    
    // Getters
    float getSpeed() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::Sprite &getSprite();
    int getHp() const;
    int getMaxHp() const;
    int getMana() const;
    int getMaxMana() const;
    sf::Vector2f getVelocity() const;
    std::array<bool, 4> getContacts() const;
    std::string getName() const;
    bool isCanDash() const;
};