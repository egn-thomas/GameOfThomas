#include "Object.hpp"

/**
 * @brief Constructeur de Object
 */
Object::Object(const std::string &name, const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture)
    : name(name), position(position), texture(std::move(texture))
{
    sprite.setTexture(*this->texture);
    sprite.setPosition(position);
    // Scaling spécifié dans les classes héritantes
    hitbox = sf::FloatRect(position.x, position.y, 32.f * 4.f, 32.f * 4.f);
}

/**
 * @brief Affiche l'objet
 */
void Object::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

/**
 * @brief Retourne les limites de l'objet
 */
sf::FloatRect Object::getBounds() const
{
    return hitbox;
}

/**
 * @brief Retourne la position de l'objet
 */
sf::Vector2f Object::getPosition() const
{
    return position;
}

/**
 * @brief Définit la position de l'objet
 */
void Object::setPosition(const sf::Vector2f &pos)
{
    position = pos;
    sprite.setPosition(position);
    hitbox.left = position.x;
    hitbox.top = position.y;
}

/**
 * @brief Retourne le nom de l'objet
 */
std::string Object::getName() const
{
    return name;
}

/**
 * @brief Définit la hitbox de l'objet
 */
void Object::setHitbox(float offsetX, float offsetY, float width, float height)
{
    hitbox = sf::FloatRect(position.x + offsetX, position.y + offsetY, width, height);
}
