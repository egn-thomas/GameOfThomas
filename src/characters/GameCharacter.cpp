#include "GameCharacter.hpp"
#include <iostream>
#include <array>

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
    contactTop = contactBottom = contactLeft = contactRight = false;
}

/**
 * @brief Mets à jour le personnage.
 *
 * @param deltatime Le temps écoulé depuis le dernier update, en secondes.
 * @param grounds Référence à un vecteur de tous les "Ground" du jeu.
 */
void GameCharacter::update(float deltaTime, const std::vector<Ground> &grounds)
{
    // Réinitialiser les états de contact
    contactTop = contactBottom = contactLeft = contactRight = false;
    onGround = false;

    // Appliquer la gravité seulement si on n’est pas au sol
    if (!contactBottom)
        applyGravity(deltaTime);

    // Calcul du déplacement prévisionnel
    sf::Vector2f moveOffset = velocity * deltaTime;
    position += moveOffset;
    sprite.setPosition(position);

    // Vérifier les collisions et mettre à jour les booléens
    checkAllCollisions(grounds);

    // Corriger la position en fonction des collisions détectées
    if (contactBottom) {
        velocity.y = 0.f;
        onGround = true;
    }
    if (contactTop)
        velocity.y = std::min(velocity.y, 0.f);
    if (contactLeft || contactRight)
        velocity.x = 0.f;

    // Repositionner le sprite final
    sprite.setPosition(position);
}

/**
 * @brief Retourne la hitBox de notre personnage sous forme d'un rectangle.
 *
 * @return FloatRect : La hit-box du personnage.
 */
const sf::FloatRect GameCharacter::getBounds() const
{
    return sprite.getGlobalBounds();
}

/**
 * @brief Cette methode appelle checkCollisionWithGround() pour chaque sol
 * 
 * @param grounds vecteur de Ground
 */
void GameCharacter::checkAllCollisions(const std::vector<Ground>& grounds)
{
    for (const auto& ground : grounds)
        checkCollisionWithGround(ground);
}

/**
 * @brief A pour objectif de verifier si le personnage touche le Ground passé en paramètres pui ajuste la position du personnage.
 * @note La fonction modifie l'état des variables de contact de la classe.
 *
 * @param ground Le ground que l'on souhaite tester
 */
void GameCharacter::checkCollisionWithGround(const Ground& ground)
{
    sf::FloatRect playerBounds = getBounds();
    sf::FloatRect groundBounds = ground.getBounds();

    if (playerBounds.intersects(groundBounds))
    {
        float overlapLeft   = (playerBounds.left + playerBounds.width) - groundBounds.left;
        float overlapRight  = (groundBounds.left + groundBounds.width) - playerBounds.left;
        float overlapTop    = (playerBounds.top + playerBounds.height) - groundBounds.top;
        float overlapBottom = (groundBounds.top + groundBounds.height) - playerBounds.top;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY)
        {
            if (overlapLeft < overlapRight)
            {
                contactRight = true;
                position.x -= overlapLeft;
            }
            else
            {
                contactLeft = true;
                position.x += overlapRight;
            }
        }
        else
        {
            if (overlapTop < overlapBottom)
            {
                contactBottom = true;
                position.y -= overlapTop;
            }
            else
            {
                contactTop = true;
                position.y += overlapBottom;
                velocity.y = 0.f;
            }
        }

        sprite.setPosition(position);
    }
}

/**
 * @brief Fonction qui applique la gravité au personnage
 *
 * @param deltatime Le temps écoulé depuis la dernière mise à jour.
 */
void GameCharacter::applyGravity(float deltaTime)
{
    if (!onGround)
        velocity.y += gravity * deltaTime;
}

/**
 * @brief Dessine le personnage
 *
 * @param window La fenetre ou on veut dessiner le personnage
 */
void GameCharacter::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

/**
 * @brief Place le personnage à la position passée en paramètres
 *
 * @param x coordonnées en x
 * @param y coordonnées en y
 */
void GameCharacter::setPosition(float x, float y)
{
    position = {x, y};
    sprite.setPosition(position);
}

/**
 * @return La position du personnage
 */
sf::Vector2f GameCharacter::getPosition() const { return position; }

/**
 * @brief Déplace le personnage selon unn vecteur
 *
 *  @param offset Le vecteur en paramètre
 */
void GameCharacter::move(const sf::Vector2f &offset)
{
    position += offset;
    sprite.setPosition(position);
}

/**
 * @return La vitesse du personnage
 */
float GameCharacter::getSpeed() const { return speed; }

/**
 * @return Retourne la texture du personnage
 */
sf::Sprite &GameCharacter::getSprite() { return sprite; }

/**
 * @return Retourne les points de vie actuels du personnage
 */
int GameCharacter::getHp() const
{
    return hp;
}

/**
 * @return Retourne le nombre maximum de points de vie du personnage
 */
int GameCharacter::getMaxHp() const
{
    return maxHp;
}

/**
 * @return Retourne le mana actuel du personnage
 */
int GameCharacter::getMana() const
{
    return mana;
}

/**
 * @return Retourne le mana maximum du personnage
 */
int GameCharacter::getMaxMana() const
{
    return maxMana;
}

/**
 * @return Retourne la vitesse actuelle (velocity) du personnage sous forme de vecteur 2D
 */
sf::Vector2f GameCharacter::getVelocity() const
{
    return velocity;
}

/**
 * @brief Retourne l'état des contacts du personnage avec son environnement.
 * 
 * @return std::array<bool, 4> : { contactTop, contactBottom, contactLeft, contactRight }
 * 
 * @note L'ordre est important :
 *       --- [0] : Haut
 *       --- [1] : Bas
 *       --- [2] : Gauche
 *       --- [3] : Droite
 */
std::array<bool, 4> GameCharacter::getContacts() const
{
    return { contactTop, contactBottom, contactLeft, contactRight };
}