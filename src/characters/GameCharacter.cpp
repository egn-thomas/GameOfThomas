#include "GameCharacter.hpp"
#include <iostream>
#include <array>
#include <memory>

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
GameCharacter::GameCharacter(const std::string &name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture)
    : name(name), hp(hp), maxHp(hp), mana(mana), maxMana(mana),
      speed(speed), position(0.f, 0.f), velocity(0.f, 0.f),
      texture(std::move(texture)), previousPosition(0.f, 0.f)
{
    sprite.setTexture(*this->texture);
    sprite.setPosition(position);
}

/**
 * @brief Mets à jour le personnage.
 *
 * @param deltatime Le temps écoulé depuis le dernier update, en secondes.
 * @param grounds Référence à un vecteur de tous les "Ground" du jeu.
 */
void GameCharacter::update(float deltaTime, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    // Réinitialiser les états de contact
    collisionsToZero();

    // DASH / Gravité / Déplacement
    applyAllForces(deltaTime);

    // Animation : frames + flip
    // Détection de mouvement horizontal réel et adaptation du cycle de marche
    selfAnimator(deltaTime);

    // Collisions
    checkAllCollisions(grounds);
    applyCollisions();

    // If we are climbing, disable gravity effects on next frame
    if (isClimbing)
    {
        // while climbing we don't want gravity to accumulate
        velocity.y = 0.f;
    }

    // Cooldowns
    allCooldowns(deltaTime);
    if (attackCooldown > 0.f)
        attackCooldown -= deltaTime;
}

//--------------------------------------------------------------------------------------
//                          Position et Mouvement
//--------------------------------------------------------------------------------------

/**
 * @brief Applique toutes les forces (dash, gravité, etc.) pour mettre à jour la position du personnage.
 *
 * @param deltaTime Le temps écoulé depuis le dernier update, en secondes.
 */
void GameCharacter::applyAllForces(float deltaTime)
{
    if (isDashing)
    {
        sf::Vector2f dashOffset(dashDirection * dashSpeed * deltaTime, velocity.y * deltaTime);
        position += dashOffset;
        dashTimer -= deltaTime;
        if (dashTimer <= 0.f)
            isDashing = false;
    }
    else
    {
        // If climbing, do not apply gravity and allow vertical movement through velocity
        if (isClimbing)
        {
            position += sf::Vector2f(velocity.x * deltaTime, velocity.y * deltaTime);
        }
        else
        {
            if (!onGround)
                this->applyGravity(deltaTime);
            position += sf::Vector2f(velocity.x * deltaTime, velocity.y * deltaTime);
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
 * @brief Déplace le personnage selon un vecteur
 *
 *  @param offset Le vecteur en paramètre
 */
void GameCharacter::move(const sf::Vector2f &offset)
{
    position += offset;
    sprite.setPosition(position);
}

/**
 * @brief Déclenche le dash d'un personnage.
 */
void GameCharacter::startDash(int direction)
{
    if (canDash && !isDashing && dashCooldown <= 0.f)
    {
        dashCooldown = dashCooldownMax;
        isDashing = true;
        dashTimer = dashDuration;
        dashDirection = direction;
        canDash = false; // sera réactivé à la prochaine collision avec le sol
    }
}

/**
 * @brief Applique les ajustements de position suite aux collisions détectées.
 */
void GameCharacter::applyCollisions()
{
    if (contactBottom)
    {
        velocity.y = 0.f;
        onGround = true;
        canDash = true;
    }
    if (contactTop)
        velocity.y = std::min(velocity.y, 0.f);
    if (contactLeft || contactRight)
        velocity.x = 0.f;

    previousPosition = position;
}

/**
 * @brief Cette methode appelle checkCollisionWithGround() pour chaque sol
 *
 * @param grounds vecteur de Ground
 */
void GameCharacter::checkAllCollisions(const std::vector<std::unique_ptr<Ground>> &grounds)
{
    // Reset ladder flag; we'll set it when we detect an overlap
    onLadder = false;

    for (const auto &ground : grounds)
    {
        // If this ground is a ladder, detect overlap but do not treat as solid
        if (ground->isLadder())
        {
            if (getBounds().intersects(ground->getBounds()))
            {
                onLadder = true;
            }
        }
        // For normal solids, perform collision response
        if (ground->isGroundSolid())
            checkCollisionWithGround(*ground);
    }
}

/**
 * @brief A pour objectif de verifier si le personnage touche le Ground passé en paramètres pui ajuste la position du personnage.
 * @note La fonction modifie l'état des variables de contact de la classe.
 *
 * @param ground Le ground que l'on souhaite tester
 */
void GameCharacter::checkCollisionWithGround(const Ground &ground)
{
    sf::FloatRect playerBounds = getBounds();
    sf::FloatRect groundBounds = ground.getBounds();

    if (playerBounds.intersects(groundBounds) && ground.isGroundSolid())
    {
        float overlapLeft = (playerBounds.left + playerBounds.width) - groundBounds.left;
        float overlapRight = (groundBounds.left + groundBounds.width) - playerBounds.left;
        float overlapTop = (playerBounds.top + playerBounds.height) - groundBounds.top;
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
 * @brief Reset tous les attributs de collisions a 0.
 */
void GameCharacter::collisionsToZero()
{
    contactTop = contactBottom = contactLeft = contactRight = false;
    onGround = false;
}

/**
 * @brief Définit la hitbox du personnage
 *
 * @param offsetX Décalage en X par rapport à la position du sprite
 * @param offsetY Décalage en Y par rapport à la position du sprite
 * @param width Largeur de la hitbox
 * @param height Hauteur de la hitbox
 */
void GameCharacter::setHitbox(float offsetX, float offsetY, float width, float height)
{
    float scaleX = sprite.getScale().x;
    float scaleY = sprite.getScale().y;
    offsetX *= scaleX;
    offsetY *= scaleY;
    width *= scaleX;
    height *= scaleY;
    hitbox = sf::FloatRect(offsetX, offsetY, width, height);
}

/**
 * @brief Retourne les limites (hitbox) du personnage sous forme de sf::FloatRect
 *
 * @return sf::FloatRect Les limites du personnage
 */
sf::FloatRect GameCharacter::getBounds() const
{
    return sf::FloatRect(
        position.x + hitbox.left,
        position.y + hitbox.top,
        hitbox.width,
        hitbox.height);
}

//--------------------------------------------------------------------------------------
//                          Animation et Visuel
//--------------------------------------------------------------------------------------

/**
 * @brief Gère l'animation du personnage lorsqu'il est immobile ou en mouvement.
 *
 * @param deltaTime Le temps écoulé depuis le dernier update, en secondes.
 */
void GameCharacter::selfAnimator(float deltaTime)
{
    walkAnimator(deltaTime);
    if (frameCount > 1)
    {
        timer += deltaTime;
        if (timer >= frameTime)
        {
            timer = 0.f;
            currentFrame = (currentFrame + 1) % frameCount;
        }

        // Calculer l’IntRect final
        sf::IntRect rect;
        rect.top = 0;
        rect.height = frameHeight;

        if (facingLeft)
        {
            rect.left = (currentFrame + 1) * frameWidth;
            rect.width = -frameWidth; // flip horizontal
        }
        else
        {
            rect.left = currentFrame * frameWidth;
            rect.width = frameWidth;
        }

        sprite.setTextureRect(rect);
    }

    // Repositionner le sprite
    sprite.setPosition(position);
}

/**
 * @brief Choisis la bonne animation de marche en fonction de la direction sur l'axe "x" du personnage concerné
 *
 * @param deltaTime Delta-time de la boucle update
 */
void GameCharacter::walkAnimator(float deltaTime)
{
    float deltaX = position.x - previousPosition.x;
    const float moveThreshold = 1.0f;

    if (std::abs(deltaX) > moveThreshold)
    {
        stillTimer = 0.f;
        setAnimationState(AnimationState::Walk);

        // Déterminer l’orientation
        facingLeft = (deltaX < 0.f);
    }
    else
    {
        stillTimer += deltaTime;
        if (stillTimer > 0.1f)
            setAnimationState(AnimationState::Idle);
    }
}

/**
 * @brief Définit la texture d'animation pour un état donné.
 *
 * @param state L'état d'animation (Idle, Walk, etc.)
 * @param texture La texture associée à cet état
 */
void GameCharacter::setAnimationTexture(AnimationState state, std::shared_ptr<sf::Texture> texture, int frameCount, int frameWidth, int frameHeight, float fps)
{
    animations[state] = {texture, frameCount, frameWidth, frameHeight, 1.f / fps};
}

/**
 * @brief Change l'état d'animation actuel du personnage.
 *
 * @param newState Le nouvel état d'animation à appliquer.
 */
void GameCharacter::setAnimationState(AnimationState newState)
{
    if (currentState != newState)
    {
        currentState = newState;
        auto it = animations.find(newState);
        if (it != animations.end())
        {
            const auto &anim = it->second;
            sprite.setTexture(*anim.texture);
            frameCount = anim.frameCount;
            frameWidth = anim.frameWidth;
            frameHeight = anim.frameHeight;
            frameTime = anim.frameTime;
            currentFrame = 0;
            timer = 0.f;
            sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        }
    }
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
 * @brief Définit les paramètres d'animation du personnage
 *
 * @param count Le nombre de frames de l'animation
 * @param width La largeur d'une frame
 * @param height La hauteur d'une frame
 * @param fps Le nombre de frames par seconde
 */
void GameCharacter::setAnimationParams(int count, int width, int height, float fps)
{
    frameCount = count;
    frameWidth = width;
    frameHeight = height;
    frameTime = 1.f / fps;
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
}

//--------------------------------------------------------------------------------------
//                          Combat et Stats
//--------------------------------------------------------------------------------------

/**
 * @brief Effectue une attaque dans une direction donnée, affectant les cibles touchées.
 *
 * @param dir La direction de l'attaque (gauche ou droite).
 * @param targets Un vecteur de pointeurs vers les cibles potentielles.
 */
void GameCharacter::attack(Direction dir, std::vector<GameCharacter *> targets)
{
    if (attackCooldown > 0.f)
        return;
    const float attackRange = 100.f;               // pixels
    const float attackHeight = getBounds().height; // même hauteur que le personnage

    // Créer une hitbox pour l'attaque
    sf::FloatRect attackBox;

    if (dir == Direction::Right)
    {
        attackBox.left = position.x + getBounds().width; // à droite du personnage
    }
    else
    {
        attackBox.left = position.x - attackRange; // à gauche
    }

    attackBox.top = position.y;
    attackBox.width = attackRange;
    attackBox.height = attackHeight;

    // Vérifier collision avec tous les cibles
    for (auto target : targets)
    {
        if (target == this)
            continue; // ne pas se toucher soi-même

        if (attackBox.intersects(target->getBounds()))
        {
            target->takeDamage(10);
            std::cout << target->getHp() << std::endl;
        }
    }

    attackCooldown = attackCooldownMax;
}

/**
 * @brief Affecte des dégats au personnage
 *
 * @param dmg Le nombre de points de dégats de l'attaque
 */
void GameCharacter::takeDamage(int dmg)
{
    hp -= dmg;
    if (hp < 0)
        hp = 0;
    isDamaged = true;
    damageTimer = 0.5f;
    sprite.setColor(sf::Color(255, 100, 100));
}

/**
 * @brief Vérifie si le personnage est encore en vie
 *
 * @return true Si le personnage a des points de vie restants
 * @return false Si le personnage n'a plus de points de vie
 */
bool GameCharacter::isAlive() const
{
    if (this->hp == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Met à jour tous les cooldowns du personnage
 */
void GameCharacter::allCooldowns(float deltaTime)
{
    if (dashCooldown > 0.f)
        dashCooldown -= deltaTime;
    if (attackCooldown > 0.f)
        attackCooldown -= deltaTime;
    if (damageTimer > 0.f)
    {
        damageTimer -= deltaTime;
    } else {
        isDamaged = false;
        sprite.setColor(sf::Color(255, 255, 255));
    }
}

//--------------------------------------------------------------------------------------
//                          Getters & Setters
//--------------------------------------------------------------------------------------

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
    return {contactTop, contactBottom, contactLeft, contactRight};
}

/**
 * @brief Retourne le nom du personnage
 *
 * @return Un string : le nom du personnage
 */
std::string GameCharacter::getName() const
{
    return name;
}

bool GameCharacter::isCanDash() const
{
    return canDash;
}