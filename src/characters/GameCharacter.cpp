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
    onGround = false;

    applyGravity(deltaTime);

    sf::Vector2f moveOffset = velocity * deltaTime;

    for (const auto& ground : grounds)
        checkCollisionWithGround(ground, moveOffset);

    position += moveOffset;
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
 * @brief A pour objectif de verifier si le personnage touche le Ground passé en paramètres pui ajuste la position du personnage.
 * 
 * @param ground Le ground que l'on souhaite tester
 * @param moveOffset Le deplacement que l'on veut appliquer au personnage
 */
void GameCharacter::checkCollisionWithGround(const Ground& ground, sf::Vector2f& moveOffset)
{
    sf::FloatRect futureBounds = getBounds();
    futureBounds.left += moveOffset.x;
    futureBounds.top  += moveOffset.y;

    sf::FloatRect groundBounds = ground.getBounds();

    if (futureBounds.intersects(groundBounds))
    {
        float playerBottom = futureBounds.top + futureBounds.height;
        float playerTop    = futureBounds.top;
        float playerLeft   = futureBounds.left;
        float playerRight  = futureBounds.left + futureBounds.width;

        float groundTop    = groundBounds.top;
        float groundBottom = groundBounds.top + groundBounds.height;
        float groundLeft   = groundBounds.left;
        float groundRight  = groundBounds.left + groundBounds.width;

        // Calculer les chevauchements
        float overlapX = std::min(playerRight - groundLeft, groundRight - playerLeft);
        float overlapY = std::min(playerBottom - groundTop, groundBottom - playerTop);

        if (overlapY < overlapX) {
            // Collision verticale
            if (velocity.y > 0.f) { // descente → sol
                moveOffset.y -= overlapY;
                velocity.y = 0.f;
                onGround = true;
            } else if (velocity.y < 0.f) { // montée → plafond
                moveOffset.y += overlapY;
                velocity.y = 0.f;
            }
        } else {
            // Collision horizontale
            if (velocity.x > 0.f) { // droite
                moveOffset.x -= overlapX;
                velocity.x = 0.f;
            } else if (velocity.x < 0.f) { // gauche
                moveOffset.x += overlapX;
                velocity.x = 0.f;
            }
        }
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