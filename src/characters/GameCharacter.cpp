#include "GameCharacter.hpp"

// Constructeur
GameCharacter::GameCharacter(const std::string &name, int hp, int mana, float speed, const sf::Texture &texture)
    : name(name), hp(hp), maxHp(hp), mana(mana), maxMana(mana), speed(speed), position(0.f, 0.f), velocity(0.f, 0.f)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void GameCharacter::update(float deltaTime, const std::vector<Ground> &grounds)
{
    if (!isOnGround())
        applyGravity(deltaTime);

    position += velocity * deltaTime;
    sprite.setPosition(position);

    onGround = false; // reset
    for (const auto &ground : grounds)
        checkCollisionWithGround(ground.getBounds());
}

void GameCharacter::checkCollisionWithGround(const sf::FloatRect &groundBounds)
{
    sf::FloatRect playerBounds = sprite.getGlobalBounds();

    if (playerBounds.intersects(groundBounds) && velocity.y > 0.f)
    {
        position.y = groundBounds.top - playerBounds.height;
        velocity.y = 0.f;
        onGround = true;
    }
}

// Draw
void GameCharacter::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

// Stats
void GameCharacter::takeDamage(int dmg)
{
    hp -= dmg;
    if (hp < 0)
        hp = 0;
}

bool GameCharacter::isAlive() const
{
    return hp > 0;
}

// Position
void GameCharacter::setPosition(float x, float y)
{
    position = {x, y};
    sprite.setPosition(position);
}

sf::Vector2f GameCharacter::getPosition() const
{
    return position;
}

void GameCharacter::move(const sf::Vector2f &offset)
{
    position += offset;
    sprite.setPosition(position);
}

void GameCharacter::applyGravity(float deltaTime)
{
    if (!onGround)
    {
        velocity.y += gravity * deltaTime;
    }
}

// Getters
int GameCharacter::getHp() const { return hp; }
int GameCharacter::getMana() const { return mana; }
float GameCharacter::getSpeed() const { return speed; }

sf::Sprite &GameCharacter::getSprite()
{
    return sprite;
}