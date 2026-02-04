#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../environnement/Ground.hpp"
#include "Direction.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <stack>
#include <array>
#include "../items/Item.hpp"

class CameraShake; // Forward declaration

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
    SwordAttack,
    CandleAttack
};

class GameCharacter
{
private:
    std::string name;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int endurance;
    int maxEndurance;
    int level;
    float speed;

    // Item-based bonuses
    int damageBonus = 0; // added by DamageAmulet

    // Inventaire : 10 piles d'items (empilables)
    static constexpr int INVENTORY_SIZE = 10;
    struct ItemStack {
        std::stack<std::unique_ptr<Item>> stack;
        std::string itemTypeName; // Pour l'affichage/type
    };
    std::array<ItemStack, INVENTORY_SIZE> inventory{};

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

    const float attackCooldownMax = 0.8f; // Durée du cooldown par défaut en secondes (utilisé si nécessaire)

    bool isDamaged = false;
    float damageTimer = 0.f;
    
    // Stun system
    float stunTimer = 0.f;
    const float stunDuration = 0.5f; // Durée du stun
    
    // Knockback timer - resets velocity.x when expired
    float knockbackTimer = 0.f;
    
    // Damage delay system - stores a single pending attack
    struct PendingAttack {
        GameCharacter* target;
        float delayTimer;
        int damage;
        int attackDirection;      // +1 = right, -1 = left
        float knockback;
        float stunDuration;
        bool isActive = false;
    };
    PendingAttack pendingAttack;
    
    // Last computed attack box for debug/visualization
    sf::FloatRect lastAttackBox{0.f,0.f,0.f,0.f};
    bool hasAttackBox = false;

    // Pushing force (for character vs character resolution)
    int force = 0; // default neutral force


    float attackBoxTimer = 0.f;
    const float attackBoxDuration = 0.2f; // show attack box for same duration as delay

    struct AttackData
    {
        // Right-side defaults
        float range = 100.f;        // horizontal reach to the right in pixels
        float topOffset = 0.f;      // offset from character.position.y for attack box top (right)
        float height = 0.f;         // if 0 => use getBounds().height (right)

        // Optional left-side overrides when asymmetric == true
        bool asymmetric = false;    // whether left-side overrides are active
        float rangeLeft = 0.f;      // horizontal reach to the left
        float topOffsetLeft = 0.f;  // top offset for left-side hitbox
        float heightLeft = 0.f;     // height for left-side hitbox (0 => same logic as right)

        int damage = 10;
        float delay = 0.f;         // delay before applying damage
        float knockback = 0.f;      // knockback force (pixels/sec)
        float stunDuration = 0.5f;  // stun duration in seconds
        float reattackDelay = 0.f;  // extra delay before next attack can be made
    };

    std::unordered_map<AttackType, AttackData> attackTypes;


protected:

    const char* FlashShaderCode = R"(
    uniform sampler2D texture;
    uniform float flashAlpha;

    void main() {
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
        // On mélange la couleur originale avec du blanc pur (1.0, 1.0, 1.0)
        // en fonction de flashAlpha (0.0 = normal, 1.0 = tout blanc)
        gl_FragColor = vec4(mix(pixel.rgb, vec3(1.0, 1.0, 1.0), flashAlpha), pixel.a);
    }
    )";

    sf::Shader flashShader;

    float attackCooldown;                 // Temps restant avant la prochaine attaque (accessible aux sous-classes)
    float minAttackCooldown = 0.05f;      // Minimum cooldown when stamina is used (can be overridden by Player)

    // Stamina / Endurance system (float internal variables; integer endurance/maxEndurance are declared in private section)
    float enduranceF = 0.f;                // internal floating point stamina
    float staminaRegenRate = 8.0f;         // stamina points regenerated per second
    int attackStaminaCost = 10;            // stamina cost per attack
    int dashStaminaCost = 20;              // stamina cost per dash

    // helper methods for stamina
    bool hasStamina(int cost) const { return enduranceF >= static_cast<float>(cost); }
    bool consumeStamina(int cost); // subtract and update endurance
    void regenStamina(float deltaTime); // regen per-frame

    bool isStunned = false;

    // Camera shake support
    CameraShake* cameraShake = nullptr;

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
    GameCharacter(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture);
    virtual ~GameCharacter() = default;

    // Gestion de l'inventaire
    bool addItem(std::unique_ptr<Item> item); // Ajoute un item (empile si même type, sinon nouvelle pile)
    bool useItem(int slot); // Utilise l'item au sommet de la pile du slot
    bool removeItem(int slot); // Retire l'item au sommet de la pile du slot
    const std::array<ItemStack, INVENTORY_SIZE>& getInventory() const { return inventory; }

    // Méthodes essentielles

    void update(float deltaTime, const std::vector<std::unique_ptr<Ground>> &grounds);

    // Ajoute des points de vie (sans dépasser le max)
    void heal(int amount);

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
    
    // Safe collision checking for pushing (prevents pushing through walls)
    bool wouldCollideWithGroundsAt(const sf::Vector2f &testPosition, const std::vector<std::unique_ptr<Ground>> &grounds) const;
    
    // Line of sight checking (raycasting) - prevents seeing/attacking through walls
    static bool hasLineOfSight(const sf::Vector2f &from, const sf::Vector2f &to, const std::vector<std::unique_ptr<Ground>> &grounds);
    
    virtual sf::FloatRect getBounds() const;
    bool isOnGround() const { return onGround; }
    bool isOnLadder() const { return onLadder; }
    void setClimbing(bool c) { isClimbing = c; }

    // Force accessors for character-vs-character collision resolution
    int getForce() const { return force; }
    void setForce(int f) { force = f; }

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

    virtual void attack(Direction dir, std::vector<GameCharacter *> targets, const std::vector<std::unique_ptr<Ground>> &grounds);
    virtual void attack(Direction dir, std::vector<GameCharacter *> targets, AttackType type, const std::vector<std::unique_ptr<Ground>> &grounds);

    // Resolve collision with another character (push weaker one out)
    // Now includes ground collision validation to prevent pushing through walls
    void resolveCollisionWithCharacter(GameCharacter &other, const std::vector<std::unique_ptr<Ground>> &grounds);

    // Gestion des stats

    void takeDamage(int dmg);
    bool isAlive() const;
    void allCooldowns(float deltaTime);

    // Camera shake support
    void setCameraShake(CameraShake* shake) { cameraShake = shake; }

    // Stat modifiers (used by items)
    void restoreMana(int amount);
    void increaseMaxHp(int amount);
    void addDamageBonus(int amount);

    // Getters
    float getSpeed() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    sf::Sprite &getSprite();
    int getHp() const;
    int getMaxHp() const;
    int getMana() const;
    int getMaxMana() const;
    int getEndurance() const;
    int getMaxEndurance() const;
    sf::Vector2f getVelocity() const;
    std::array<bool, 4> getContacts() const;
    std::string getName() const;
    bool isCanDash() const;
    bool hasAttackHitbox() const;
    sf::FloatRect getAttackHitbox() const;
    // Attack type configuration
    void setAttackTypeParams(AttackType type, float range, float topOffset, float height, int damage, float delay, float knockback, float stunDuration);
    GameCharacter::AttackData getAttackTypeParams(AttackType type) const;

    // Configure attack type params with asymmetric overrides (left/right)
    void setAttackTypeParamsAsymmetric(AttackType type,
                                     float rangeRight, float topOffsetRight, float heightRight,
                                     float rangeLeft, float topOffsetLeft, float heightLeft,
                                     int damage, float delay, float knockback, float stunDuration);
};