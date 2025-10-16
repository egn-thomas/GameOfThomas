#include "GameCharacter.hpp"

/**
 * @brief Constructeur de GameCharacter, constitue la classe mère de tous les personnages du jeu.
 * 
 * @param name Nom du personnage
 * @param hp Les points de vie maximales du personnage
 * @param mana Le mana maximal du personnage
 * @param speed La vitesse du personnage
 * @param texture Son skin dans le jeu
 * 
 * @note Par défaut, le personnage est placé à (0, 0)
 */
GameCharacter::GameCharacter(const std::string &name, int hp, int mana, float speed, const sf::Texture &texture)
    : name(name), hp(hp), maxHp(hp), mana(mana), maxMana(mana),
      speed(speed), position(0.f, 0.f), velocity(0.f, 0.f), onGround(false)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

/**
 * @brief Mets à jour le personnage.
 * 
 * @param deltatime Le temps écoulé depuis le dernier update, en secondes.
 * @param grounds Référence à un vecteur de tous les "Ground" du jeu.
 */
void GameCharacter::update(float deltaTime, const std::vector<Ground>& grounds)
{
    if (!onGround)
        applyGravity(deltaTime);

    position += velocity * deltaTime;
    sprite.setPosition(position);

    onGround = false;

    for (const auto& ground : grounds)
        checkCollisionWithGround(ground);
}

const sf::FloatRect GameCharacter::getBounds() const
{
    return sprite.getGlobalBounds();
}

void GameCharacter::checkCollisionWithGround(const Ground& ground)
{
    sf::FloatRect playerBounds = getBounds();
    sf::FloatRect groundBounds = ground.getBounds();

    if (playerBounds.intersects(groundBounds))
    {
        float playerBottom = playerBounds.top + playerBounds.height;
        float playerTop = playerBounds.top;
        float playerLeft = playerBounds.left;
        float playerRight = playerBounds.left + playerBounds.width;

        float groundTop = groundBounds.top;
        float groundBottom = groundBounds.top + groundBounds.height;
        float groundLeft = groundBounds.left;
        float groundRight = groundBounds.left + groundBounds.width;

        float overlapLeft = playerRight - groundLeft;
        float overlapRight = groundRight - playerLeft;
        float overlapTop = playerBottom - groundTop;
        float overlapBottom = groundBottom - playerTop;

        bool fromLeft = std::abs(overlapLeft) < std::abs(overlapRight);
        bool fromTop = std::abs(overlapTop) < std::abs(overlapBottom);

        float minX = fromLeft ? overlapLeft : -overlapRight;
        float minY = fromTop ? overlapTop : -overlapBottom;

        // Collision horizontale
        if (std::abs(minX) < std::abs(minY))
        {
            sprite.move(-minX, 0.f);
            velocity.x = 0.f;
        }
        // Collision verticale
        else
        {
            sprite.move(0.f, -minY);

            if (minY < 0.f) // touche le dessus du sol
            {
                onGround = true;
                velocity.y = 0.f;
            }
            else
            {
                // touche par le bas (plafond), on stoppe le mouvement vertical
                velocity.y = std::min(velocity.y, 0.f);
            }
        }
    }
}

void GameCharacter::applyGravity(float deltaTime)
{
    const float gravity = 2000.f; // pixels/s²
    if (!onGround)
        velocity.y += gravity * deltaTime;
}

// ----------------------------------

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

bool GameCharacter::isAlive() const { return hp > 0; }

// Position
void GameCharacter::setPosition(float x, float y)
{
    position = {x, y};
    sprite.setPosition(position);
}

sf::Vector2f GameCharacter::getPosition() const { return position; }

void GameCharacter::move(const sf::Vector2f &offset)
{
    position += offset;
    sprite.setPosition(position);
}

// Getters
int GameCharacter::getHp() const { return hp; }
int GameCharacter::getMana() const { return mana; }
float GameCharacter::getSpeed() const { return speed; }

sf::Sprite &GameCharacter::getSprite() { return sprite; }
