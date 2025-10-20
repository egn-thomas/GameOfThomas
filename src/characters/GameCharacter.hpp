#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../environnement/Ground.hpp"
#include "Direction.hpp"
#include <memory>

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
    
    // Pour gérer l'animation :
    
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
    
    protected:
    std::shared_ptr<sf::Texture> texture;
    bool onGround = false;
    sf::Vector2f velocity;
    sf::Sprite sprite;
    sf::FloatRect hitbox;  
    
    public:
    GameCharacter(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);
    virtual ~GameCharacter() = default;
    
    // Méthodes essentielles

    void update(float deltaTime, const std::vector<std::unique_ptr<Ground>> &grounds);
    
    // Visuel et animation
    
    virtual void draw(sf::RenderWindow &window);
    void setAnimationParams(int frameCount, int frameWidth, int frameHeight, float fps);
    
    // Position et mouvement
    
    void setPosition(float x, float y);
    void move(const sf::Vector2f &offset);
    void applyGravity(float deltaTime);
    bool isOnGround() const { return onGround; }
    virtual sf::FloatRect getBounds() const;
    void checkAllCollisions(const std::vector<std::unique_ptr<Ground>> &grounds);
    void checkCollisionWithGround(const Ground &ground); 
    void startDash(int direction);
    void setHitbox(float offsetX, float offsetY, float width, float height);
    
    // Combat
    
    void attack(Direction dir, std::vector<GameCharacter *> targets);
    
    // Gestion des stats
    
    void takeDamage(int dmg);
    bool isAlive() const;
    
    // Getters
    float getSpeed() const;
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