#include "GameCharacter.hpp"
#include "../ui/CameraShake.hpp"
#include <iostream>
#include <array>
#include <memory>
#include <cmath>

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
GameCharacter::GameCharacter(const std::string &name, int hp, int mana, int stamina, float speed, std::shared_ptr<sf::Texture> texture)
    : name(name), hp(hp), maxHp(hp), mana(mana), maxMana(mana),
      endurance(stamina), maxEndurance(stamina),speed(speed), position(0.f, 0.f), velocity(0.f, 0.f),
      texture(std::move(texture)), previousPosition(0.f, 0.f)
{
    if (!flashShader.loadFromMemory(FlashShaderCode, sf::Shader::Fragment)) {
        std::cerr << "Failed to load flash shader from memory!" << std::endl;
    }
    sprite.setTexture(*this->texture);
    sprite.setPosition(position);

    // initialize internal float stamina tracker
    enduranceF = static_cast<float>(endurance);
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
    // Do not allow movement while stunned
    if (isStunned)
        return;
    position += offset;
    sprite.setPosition(position);
}

/**
 * @brief Déclenche le dash d'un personnage.
 */
void GameCharacter::startDash(int direction)
{
    // cannot dash while stunned
    if (isStunned)
        return;

    // require enough stamina to dash
    if (!hasStamina(dashStaminaCost))
        return;

    if (canDash && !isDashing && dashCooldown <= 0.f)
    {
        // consume stamina up-front
        consumeStamina(dashStaminaCost);

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
 * @brief Checks if a character at a given position would collide with any solid ground.
 * Used to validate safe positions when pushing characters.
 * 
 * @param testPosition The position to test
 * @param grounds The vector of all solid grounds
 * @return true if the position would cause a collision, false if safe
 */
bool GameCharacter::wouldCollideWithGroundsAt(const sf::Vector2f &testPosition, const std::vector<std::unique_ptr<Ground>> &grounds) const
{
    // Temporarily create a bounds object at the test position
    sf::FloatRect testBounds = getBounds();
    testBounds.left = testPosition.x + (getBounds().left - position.x);
    testBounds.top = testPosition.y + (getBounds().top - position.y);
    
    // Check intersection with all solid grounds
    for (const auto &ground : grounds)
    {
        if (ground->isGroundSolid() && testBounds.intersects(ground->getBounds()))
        {
            return true; // Would collide
        }
    }
    return false; // Safe position
}

/**
 * @brief Raycasting function to check if there's a clear line of sight between two points.
 * Tests if a straight line intersects any solid ground obstacles.
 * Uses a simpler approach: just check if the line segment intersects any solid obstacle.
 * 
 * @param from Starting position
 * @param to Target position
 * @param grounds Vector of all solid grounds
 * @return true if line of sight is clear, false if blocked by walls
 */
bool GameCharacter::hasLineOfSight(const sf::Vector2f &from, const sf::Vector2f &to, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    // If positions are the same, there's line of sight
    sf::Vector2f diff = to - from;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    
    if (distance < 1.f) // positions effectively the same
        return true;
    
    // Normalize the direction vector
    sf::Vector2f direction = diff / distance;
    
    // Sample along the line with larger steps to avoid excessive collision detection
    // This prevents false negatives from tight corridors near ladders
    const float stepSize = 10.f; // Check every 10 pixels
    const float rayRadius = 15.f; // Radius around the line (character body width ~30px)
    
    // Sample from the start to the end
    for (float t = stepSize; t < distance; t += stepSize)
    {
        sf::Vector2f samplePoint = from + direction * t;
        
        // Create bounds around the sample point (larger radius for more leniency)
        sf::FloatRect rayBounds(samplePoint.x - rayRadius / 2.f, 
                               samplePoint.y - rayRadius / 2.f,
                               rayRadius, rayRadius);
        
        // Check intersection with solid grounds (ladders are non-solid, so automatically excluded)
        for (const auto &ground : grounds)
        {
            // Only block if it's a solid ground AND not a ladder
            if (ground->isGroundSolid() && !ground->isLadder())
            {
                // Check if ray intersects this ground
                if (rayBounds.intersects(ground->getBounds()))
                {
                    return false; // Line of sight is blocked
                }
            }
        }
    }
    
    return true; // Line of sight is clear
}

    // Resolve collision between two characters: stronger pushes weaker; equal -> no movement
    // Now validates that the push doesn't move a character through walls/terrain
    void GameCharacter::resolveCollisionWithCharacter(GameCharacter &other, const std::vector<std::unique_ptr<Ground>> &grounds)
    {
        sf::FloatRect a = getBounds();
        sf::FloatRect b = other.getBounds();
        if (!a.intersects(b))
            return;

        float overlapLeft = (a.left + a.width) - b.left;
        float overlapRight = (b.left + b.width) - a.left;
        float overlapTop = (a.top + a.height) - b.top;
        float overlapBottom = (b.top + b.height) - a.top;

        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (force > other.force)
        {
            // push other out of overlap - with collision validation
            if (minOverlapX < minOverlapY)
            {
                // Horizontal push
                sf::Vector2f testPos = other.position;
                if (overlapLeft < overlapRight)
                    testPos.x += overlapLeft;
                else
                    testPos.x -= overlapRight;
                
                // Only apply if it doesn't cause collision with ground
                if (!other.wouldCollideWithGroundsAt(testPos, grounds))
                {
                    other.position = testPos;
                }
            }
            else
            {
                // Vertical push
                sf::Vector2f testPos = other.position;
                if (overlapTop < overlapBottom)
                    testPos.y += overlapTop;
                else
                {
                    testPos.y -= overlapBottom;
                    other.velocity.y = 0.f;
                }
                
                // Only apply if it doesn't cause collision with ground
                if (!other.wouldCollideWithGroundsAt(testPos, grounds))
                {
                    other.position = testPos;
                }
            }
            other.sprite.setPosition(other.position);
        }
        else if (force < other.force)
        {
            // other pushes this out - with collision validation
            if (minOverlapX < minOverlapY)
            {
                // Horizontal push
                sf::Vector2f testPos = position;
                if (overlapLeft < overlapRight)
                    testPos.x -= overlapLeft;
                else
                    testPos.x += overlapRight;
                
                // Only apply if it doesn't cause collision with ground
                if (!wouldCollideWithGroundsAt(testPos, grounds))
                {
                    position = testPos;
                }
            }
            else
            {
                // Vertical push
                sf::Vector2f testPos = position;
                if (overlapTop < overlapBottom)
                    testPos.y -= overlapTop;
                else
                {
                    testPos.y += overlapBottom;
                    velocity.y = 0.f;
                }
                
                // Only apply if it doesn't cause collision with ground
                if (!wouldCollideWithGroundsAt(testPos, grounds))
                {
                    position = testPos;
                }
            }
            sprite.setPosition(position);
        }
        else
        {
            // equal force -> no net movement: revert both to previous positions and zero velocities
            position = previousPosition;
            other.position = other.previousPosition;
            velocity = sf::Vector2f(0.f, 0.f);
            other.velocity = sf::Vector2f(0.f, 0.f);
            sprite.setPosition(position);
            other.sprite.setPosition(other.position);
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
    // store raw hitbox (unscaled) as default for animations
    defaultHitboxRaw = sf::FloatRect(offsetX, offsetY, width, height);
    // apply sprite scale to compute actual hitbox used for collisions
    float scaleX = sprite.getScale().x;
    float scaleY = sprite.getScale().y;
    hitbox = sf::FloatRect(offsetX * scaleX, offsetY * scaleY, width * scaleX, height * scaleY);
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

            // Si animation d'attaque terminée, revenir à Idle mais conserver facingLeft
            if ((currentState == AnimationState::AttackLeft || currentState == AnimationState::AttackRight) && currentFrame == 0)
            {
                setAnimationState(AnimationState::Idle);
                // facingLeft est déjà set par attackAnimator, pas besoin de le modifier
            }
        }

        // Calculer l'IntRect final
        sf::IntRect rect;
        rect.top = 0;
        rect.height = frameHeight;

        // Si on est en Idle et qu'on fait face à gauche, retourner le sprite
        if (currentState == AnimationState::Idle && facingLeft)
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
    if (isStunned)
        return;
    // Ne pas modifier l'animation si on est en train d'attaquer
    if (currentState == AnimationState::AttackLeft || currentState == AnimationState::AttackRight)
        return;

    float deltaX = position.x - previousPosition.x;
    const float moveThreshold = 1.0f;

    if (std::abs(deltaX) > moveThreshold)
    {
        stillTimer = 0.f;
        // Déterminer l’orientation
        facingLeft = (deltaX < 0.f);
        if (facingLeft)
            setAnimationState(AnimationState::WalkLeft);
        else
            setAnimationState(AnimationState::WalkRight);
    }
    else
    {
        stillTimer += deltaTime;
        if (stillTimer > 0.1f)
            setAnimationState(AnimationState::Idle);
    }
}

/**
 * @brief Choisis la bonne animation d'attaque en fonction de la direction sur l'axe "x" du personnage concerné
 * @param deltaTime Delta-time de la boucle update
 * @param direction Direction de l'attaque (-1 = gauche, +1 = droite)
 */
void GameCharacter::attackAnimator(float deltaTime, Direction direction)
{
    // Déterminer l’orientation
    facingLeft = (direction == Direction::Left);
    // Mettre l'animation d'attaque
    if (facingLeft)
        setAnimationState(AnimationState::AttackLeft);
    else
        setAnimationState(AnimationState::AttackRight);
}

/**
 * @brief Définit la texture d'animation pour un état donné.
 *
 * @param state L'état d'animation (Idle, WalkLeft, WalkRight, etc.)
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
            // Update hitbox for this animation if a specific one exists, otherwise use default
            auto hitIt = animationHitboxesRaw.find(newState);
            sf::FloatRect raw;
            if (hitIt != animationHitboxesRaw.end())
            {
                raw = hitIt->second;
            }
            else
            {
                raw = defaultHitboxRaw;
            }
            // apply current sprite scale
            float scaleX = sprite.getScale().x;
            float scaleY = sprite.getScale().y;
            hitbox = sf::FloatRect(raw.left * scaleX, raw.top * scaleY, raw.width * scaleX, raw.height * scaleY);
            // Update sprite offset for this animation
            auto offsetIt = animationSpriteOffsetsRaw.find(newState);
            if (offsetIt != animationSpriteOffsetsRaw.end())
            {
                currentSpriteOffset = offsetIt->second * sprite.getScale().x; // scale offset by sprite scale
            }
            else
            {
                currentSpriteOffset = {0.f, 0.f};
            }
        }
    }
}

void GameCharacter::setAnimationHitbox(AnimationState state, float offsetX, float offsetY, float width, float height)
{
    animationHitboxesRaw[state] = sf::FloatRect(offsetX, offsetY, width, height);
    // if currently in this state, apply immediately
    if (currentState == state)
    {
        float scaleX = sprite.getScale().x;
        float scaleY = sprite.getScale().y;
        hitbox = sf::FloatRect(offsetX * scaleX, offsetY * scaleY, width * scaleX, height * scaleY);
    }
}

void GameCharacter::setAnimationSpriteOffset(AnimationState state, float offsetX, float offsetY)
{
    animationSpriteOffsetsRaw[state] = {offsetX, offsetY};
    // if currently in this state, apply immediately
    if (currentState == state)
    {
        currentSpriteOffset = {offsetX * sprite.getScale().x, offsetY * sprite.getScale().y};
    }
}

/**
 * @brief Dessine le personnage
 *
 * @param window La fenetre ou on veut dessiner le personnage
 */
void GameCharacter::draw(sf::RenderWindow &window)
{
    // Apply sprite offset for rendering
    sf::Vector2f originalPos = sprite.getPosition();
    sprite.setPosition(originalPos + currentSpriteOffset);
    // If damaged, use flash shader
    if (isDamaged && damageTimer > 0.f)
    {
        window.draw(sprite, &flashShader);
    }
    else
    {
        window.draw(sprite);
    }
    sprite.setPosition(originalPos);
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
void GameCharacter::attack(Direction dir, std::vector<GameCharacter *> targets, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    // default to SwordAttack for backward compatibility
    attack(dir, targets, AttackType::SwordAttack, grounds);
}

void GameCharacter::attack(Direction dir, std::vector<GameCharacter *> targets, AttackType type, const std::vector<std::unique_ptr<Ground>> &grounds)
{
    // cannot attack while stunned
    if (isStunned)
        return;

    if (attackCooldown > 0.f)
        return;
    // look up attack type parameters (fallback to defaults if not set)
    AttackData data;
    auto it = attackTypes.find(type);
    if (it != attackTypes.end())
        data = it->second;

    float attackRange = data.range;
    float attackTopOffset = data.topOffset;
    float attackHeight = (data.height > 0.f) ? data.height : getBounds().height;
    // If asymmetric overrides are present and attacking left, use left-side params
    if (dir == Direction::Left && data.asymmetric)
    {
        if (data.rangeLeft > 0.f) attackRange = data.rangeLeft;
        attackTopOffset = data.topOffsetLeft;
        if (data.heightLeft > 0.f) attackHeight = data.heightLeft;
    }

    // Check stamina cost before enqueuing hits (default: GameCharacter does not block attacks; Player overrides)
    // Note: we don't consume stamina here to allow Player override to manage cooldowns differently.

    float attackTop = position.y + attackTopOffset;
    int damage = data.damage + damageBonus;
    float delay = data.delay;
    float knockback = data.knockback;
    float stunDuration = data.stunDuration;
    int attackDirection = (dir == Direction::Right) ? 1 : -1;

    attackAnimator(0.f, dir);

    // Trigger camera shake on attack with the attack delay
    if (cameraShake != nullptr)
    {
        cameraShake->trigger(0.15f, 4.f, 15.f, delay); // duration, intensity, frequency, delay
    }

    // Créer une hitbox pour l'attaque en fonction du type
    sf::FloatRect attackBox;
    if (dir == Direction::Right)
        attackBox.left = position.x + getBounds().width; // à droite du personnage
    else
        attackBox.left = position.x - attackRange; // à gauche

    attackBox.top = attackTop;
    attackBox.width = attackRange;
    attackBox.height = attackHeight;

    // Vérifier collision avec la cible et créer une attaque en attente
    for (auto target : targets)
    {
        if (target == this)
            continue; // ne pas se toucher soi-même

        // Check both attack box intersection AND line of sight
        if (attackBox.intersects(target->getBounds()) && 
            hasLineOfSight(position, target->getPosition(), grounds))
        {
            // Créer une attaque en attente avec délai
            pendingAttack = {target, delay, damage, attackDirection, knockback, stunDuration, true};
            // std::cout << "Attack queued for " << target->getName() << " in " << delay << "s" << std::endl;
            break; // Une seule attaque à la fois
        }
    }

    // store attack box for debug display for a short duration
    lastAttackBox = attackBox;
    hasAttackBox = true;
    attackBoxTimer = (delay > 0.f) ? delay : attackBoxDuration;

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
    // apply stun
    isStunned = true;
    stunTimer = stunDuration;
    // sprite color will be updated in update() based on damageTimer
    
    // Trigger camera shake on damage
    if (cameraShake != nullptr)
    {
        cameraShake->trigger(0.3f, 8.f, 12.f); // duration, intensity, frequency
    }
}

bool GameCharacter::consumeStamina(int cost)
{
    if (enduranceF >= static_cast<float>(cost))
    {
        enduranceF -= static_cast<float>(cost);
        if (enduranceF < 0.f) enduranceF = 0.f;
        endurance = static_cast<int>(enduranceF);
        return true;
    }
    return false;
}

void GameCharacter::regenStamina(float deltaTime)
{
    if (enduranceF >= static_cast<float>(maxEndurance))
    {
        enduranceF = static_cast<float>(maxEndurance);
        endurance = maxEndurance;
        return;
    }
    enduranceF += staminaRegenRate * deltaTime;
    if (enduranceF > static_cast<float>(maxEndurance)) enduranceF = static_cast<float>(maxEndurance);
    endurance = static_cast<int>(enduranceF);
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
    // Process pending delayed attack
    if (pendingAttack.isActive)
    {
        pendingAttack.delayTimer -= deltaTime;
        if (pendingAttack.delayTimer <= 0.f)
        {
            if (pendingAttack.target)
            {
                // Vérifier que la cible est toujours à proximité avant d'appliquer les dégâts
                sf::FloatRect attackBox;
                float range = 100.f; // Default range
                if (pendingAttack.attackDirection == 1)
                    attackBox.left = position.x + getBounds().width;
                else
                    attackBox.left = position.x - range;
                
                attackBox.top = position.y;
                attackBox.width = range;
                attackBox.height = getBounds().height;

                if (attackBox.intersects(pendingAttack.target->getBounds()))
                {
                    pendingAttack.target->takeDamage(pendingAttack.damage);
                    // Apply knockback
                    if (pendingAttack.knockback > 0.f)
                    {
                        pendingAttack.target->velocity.x = pendingAttack.attackDirection * pendingAttack.knockback;
                        pendingAttack.target->knockbackTimer = pendingAttack.stunDuration;
                    }
                    // Override stun duration with attack's stun duration if greater
                    if (pendingAttack.stunDuration > pendingAttack.target->stunTimer)
                    {
                        pendingAttack.target->stunTimer = pendingAttack.stunDuration;
                    }
                }
            }
            pendingAttack.isActive = false;
        }
    }

    // Attack box display timer
    if (hasAttackBox)
    {
        attackBoxTimer -= deltaTime;
        if (attackBoxTimer <= 0.f)
        {
            hasAttackBox = false;
            lastAttackBox = sf::FloatRect(0.f,0.f,0.f,0.f);
        }
    }

    if (dashCooldown > 0.f)
        dashCooldown -= deltaTime;
    if (attackCooldown > 0.f)
        attackCooldown -= deltaTime;

    // Regenerate stamina every frame
    regenStamina(deltaTime);

    // Knockback timer - reset horizontal velocity when knockback expires
    if (knockbackTimer > 0.f)
    {
        knockbackTimer -= deltaTime;
        if (knockbackTimer <= 0.f)
        {
            velocity.x = 0.f; // stop knockback movement
        }
    }

    // Stun
    if (stunTimer > 0.f)
    {
        stunTimer -= deltaTime;
        if (stunTimer <= 0.f)
            isStunned = false;
    }

    if (damageTimer > 0.f)
    {
        damageTimer -= deltaTime;
        float alpha = (static_cast<int>(damageTimer * 15.f) % 2 == 0) ? 1.0f : 0.0f;
        flashShader.setUniform("flashAlpha", alpha);
    }
    else
    {
        isDamaged = false;
        sprite.setColor(sf::Color(255, 255, 255));
    }
}

//------------------------------------------------------------------------------
// Stat modifiers used by items
//------------------------------------------------------------------------------

void GameCharacter::restoreMana(int amount)
{
    mana += amount;
    if (mana > maxMana) mana = maxMana;
}

void GameCharacter::increaseMaxHp(int amount)
{
    maxHp += amount;
    hp += amount; // also give current hp
}

void GameCharacter::addDamageBonus(int amount)
{
    damageBonus += amount;
}
//--------------------------------------------------------------------------------------
//                          Getters & Setters
//--------------------------------------------------------------------------------------

void GameCharacter::heal(int amount)
{
    hp += amount;
    if (hp > maxHp)
        hp = maxHp;
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
 * @return Retourne l'endurance actuelle du personnage
 */
int GameCharacter::getEndurance() const
{
    return endurance;
}

/**
 * @return Retourne l'endurance maximale du personnage
 */
int GameCharacter::getMaxEndurance() const
{
    return maxEndurance;
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

bool GameCharacter::hasAttackHitbox() const
{
    return hasAttackBox;
}

sf::FloatRect GameCharacter::getAttackHitbox() const
{
    return lastAttackBox;
}

void GameCharacter::setAttackTypeParams(AttackType type, float range, float topOffset, float height, int damage, float delay, float knockback, float stunDuration)
{
    AttackData d;
    d.range = range;
    d.topOffset = topOffset;
    d.height = height;
    d.asymmetric = false;
    d.rangeLeft = 0.f;
    d.topOffsetLeft = 0.f;
    d.heightLeft = 0.f;
    d.damage = damage;
    d.delay = delay;
    d.knockback = knockback;
    d.stunDuration = stunDuration;
    attackTypes[type] = d;
}

void GameCharacter::setAttackTypeParamsAsymmetric(AttackType type,
                                                  float rangeRight, float topOffsetRight, float heightRight,
                                                  float rangeLeft, float topOffsetLeft, float heightLeft,
                                                  int damage, float delay, float knockback, float stunDuration)
{
    AttackData d;
    d.range = rangeRight;
    d.topOffset = topOffsetRight;
    d.height = heightRight;

    d.asymmetric = true;
    d.rangeLeft = rangeLeft;
    d.topOffsetLeft = topOffsetLeft;
    d.heightLeft = heightLeft;

    d.damage = damage;
    d.delay = delay;
    d.knockback = knockback;
    d.stunDuration = stunDuration;

    attackTypes[type] = d;
}

GameCharacter::AttackData GameCharacter::getAttackTypeParams(AttackType type) const
{
    auto it = attackTypes.find(type);
    if (it != attackTypes.end())
        return it->second;
    return AttackData();
}

// Ajoute un item à l'inventaire (empile si même type, sinon nouvelle pile)
bool GameCharacter::addItem(std::unique_ptr<Item> item) {
    std::string typeName = item->getName();
    // Chercher une pile du même type
    for (auto& stack : inventory) {
        if (!stack.stack.empty() && stack.itemTypeName == typeName) {
            stack.stack.push(std::move(item));
            return true;
        }
    }
    // Chercher une pile vide
    for (auto& stack : inventory) {
        if (stack.stack.empty()) {
            stack.itemTypeName = typeName;
            stack.stack.push(std::move(item));
            return true;
        }
    }
    // Inventaire plein
    return false;
}

// Utilise l'item au sommet de la pile du slot (et le retire)
bool GameCharacter::useItem(int slot) {
    if (slot < 0 || slot >= INVENTORY_SIZE) return false;
    auto& stack = inventory[slot];
    if (stack.stack.empty()) return false;
    stack.stack.top()->applyEffect(*this);
    stack.stack.pop();
    if (stack.stack.empty()) stack.itemTypeName = "";
    return true;
}

// Retire l'item au sommet de la pile du slot (sans effet)
bool GameCharacter::removeItem(int slot) {
    if (slot < 0 || slot >= INVENTORY_SIZE) return false;
    auto& stack = inventory[slot];
    if (stack.stack.empty()) return false;
    stack.stack.pop();
    if (stack.stack.empty()) stack.itemTypeName = "";
    return true;
}