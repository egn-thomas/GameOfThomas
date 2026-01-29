#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../environnement/Ground.hpp"
#include "Direction.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

enum class AnimationState
{
    Idle,
    WalkRight,
    WalkLeft,
    AttackLeft,
    AttackRight
};

enum class AttackType
{
    SwordAttack
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
    // Ladder / climbing state
    bool onLadder = false;   // true when overlapping a LadderGround
    bool isClimbing = false; // true while actively climbing (disables gravity)

    // Direction du personnage
    bool facingLeft = false;

    // --- Dash ---
    bool isDashing = false;
    bool canDash = true;
    float dashSpeed = 400.f;
    float dashDuration = 0.15f;
    float dashTimer = 0.f;
    int dashDirection = 0; // -1 = gauche, +1 = droite
    float dashCooldown;
    const float dashCooldownMax = 1.0f; // Durée du cooldown en secondes

    // Attack

    float attackCooldown;                 // Temps restant avant la prochaine attaque
    const float attackCooldownMax = 0.5f; // Durée du cooldown en secondes

    bool isDamaged = false;
    float damageTimer = 0.f;
    
    // Stun system
    bool isStunned = false;
    float stunTimer = 0.f;
    const float stunDuration = 0.5f; // Durée du stun
    
    // Damage delay system
    struct PendingHit {
        GameCharacter* target;
        float delayTimer;
        int damage;
    };
    std::vector<PendingHit> pendingHits;
    const float damageDelayDuration = 0.2f;
    // Last computed attack box for debug/visualization
    sf::FloatRect lastAttackBox{0.f,0.f,0.f,0.f};
    bool hasAttackBox = false;
    float attackBoxTimer = 0.f;
    const float attackBoxDuration = damageDelayDuration; // show attack box for same delay

    struct AttackData
    {
        float range = 100.f;     // horizontal reach in pixels
        float topOffset = 0.f;   // offset from character.position.y for attack box top
        float height = 0.f;      // if 0 => use getBounds().height
        int damage = 10;
        float delay = 0.2f;      // delay before applying damage
    };

    std::unordered_map<AttackType, AttackData> attackTypes;
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
    // Raw hitbox definitions (values before sprite scaling)
    sf::FloatRect defaultHitboxRaw{0.f, 0.f, 0.f, 0.f};
    std::unordered_map<AnimationState, sf::FloatRect> animationHitboxesRaw;
    // Sprite visual offset per animation (for rendering only, doesn't affect position/collision)
    sf::Vector2f currentSpriteOffset{0.f, 0.f};
    std::unordered_map<AnimationState, sf::Vector2f> animationSpriteOffsetsRaw;

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
    bool isOnLadder() const { return onLadder; }
    void setClimbing(bool c) { isClimbing = c; }

    // Visuel et animation

    void selfAnimator(float deltaTime);
    void walkAnimator(float deltaTime);
    void attackAnimator(float deltaTime, Direction direction);
    void setAnimationTexture(AnimationState state, std::shared_ptr<sf::Texture> texture, int frameCount, int frameWidth, int frameHeight, float fps);
    void setAnimationState(AnimationState newState);
    void setAnimationHitbox(AnimationState state, float offsetX, float offsetY, float width, float height);
    void setAnimationSpriteOffset(AnimationState state, float offsetX, float offsetY);
    virtual void draw(sf::RenderWindow &window);
    void setAnimationParams(int frameCount, int frameWidth, int frameHeight, float fps);

    // Combat

    void attack(Direction dir, std::vector<GameCharacter *> targets);
    void attack(Direction dir, std::vector<GameCharacter *> targets, AttackType type);

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
    bool hasAttackHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    // Attack type configuration
    void setAttackTypeParams(AttackType type, float range, float topOffset, float height, int damage, float delay);
    GameCharacter::AttackData getAttackTypeParams(AttackType type) const;
};