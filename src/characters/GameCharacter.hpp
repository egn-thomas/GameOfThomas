#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GameCharacter {
protected:
    std::string name;
    int hp;
    int maxHp;
    int mana;
    int maxMana;
    int level;
    float speed;

    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Sprite sprite;

public:
    GameCharacter(const std::string& name, int hp, int mana, float speed, const sf::Texture& texture);
    virtual ~GameCharacter() = default;

    // Méthodes essentielles
    virtual void update() = 0;

    virtual void draw(sf::RenderWindow& window);

    // Gestion des stats
    void takeDamage(int dmg);

    bool isAlive() const;

    // Position et mouvement
    void setPosition(float x, float y);
    void move(const sf::Vector2f& offset);
    
    // Getters
    int getHp() const;
    int getMana() const;
    float getSpeed() const;
    sf::Vector2f getPosition() const;
    sf::Sprite& getSprite();
};