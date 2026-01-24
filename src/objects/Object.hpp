#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

/**
 * @brief Classe de base pour tous les objets du jeu (coffres, pièges, etc.)
 */
class Object
{
protected:
    std::string name;
    sf::Vector2f position;
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    sf::FloatRect hitbox;

public:
    /**
     * @brief Constructeur de Object
     * @param name Nom de l'objet
     * @param position Position de l'objet dans le monde
     * @param texture Texture de l'objet
     */
    Object(const std::string &name, const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture);
    
    virtual ~Object() = default;

    /**
     * @brief Affiche l'objet
     */
    virtual void draw(sf::RenderWindow &window);

    /**
     * @brief Retourne les limites de l'objet
     */
    virtual sf::FloatRect getBounds() const;

    /**
     * @brief Retourne la position de l'objet
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Définit la position de l'objet
     */
    void setPosition(const sf::Vector2f &pos);

    /**
     * @brief Retourne le nom de l'objet
     */
    std::string getName() const;

    /**
     * @brief Définit la hitbox de l'objet
     */
    void setHitbox(float offsetX, float offsetY, float width, float height);
};
