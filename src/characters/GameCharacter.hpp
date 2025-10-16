#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../environnement/Ground.hpp"

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
    sf::Sprite sprite;

    float gravity = 980.f;

protected:
    bool onGround = false;
    sf::Vector2f velocity;

public:
    GameCharacter(const std::string &name, int hp, int mana, float speed, const sf::Texture &texture);
    virtual ~GameCharacter() = default;

    // Méthodes essentielles
    void update(float deltaTime, const std::vector<Ground> &grounds);
    virtual void draw(sf::RenderWindow &window);

    // Position et mouvement
    void setPosition(float x, float y);
    void move(const sf::Vector2f &offset);
    void applyGravity(float deltaTime);
    bool isOnGround() const { return onGround; }
    const sf::FloatRect getBounds() const;
    void checkCollisionWithGround(const Ground& ground, sf::Vector2f& moveOffset);

    // Gestion des stats
    void takeDamage(int dmg);
    bool isAlive() const;

    // Getters
    float getSpeed() const;
    sf::Vector2f getPosition() const;
    sf::Sprite &getSprite();
};