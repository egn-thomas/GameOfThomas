/**
 * @file IMPLEMENTATION_IA.md
 * @brief Guide d'implémentation de la logique IA pour les ennemis
 * 
 * # Architecture IA
 * 
 * ## Classes principales
 * 
 * ### NonPlayer (classe mère)
 * - Classe de base pour tous les ennemis
 * - Possède une méthode virtuelle `updateBehavior()` à surcharger
 * - Interface commune pour tous les comportements IA
 * 
 * ### Candle (première implémentation)
 * - Enemi qui poursuit le joueur
 * - Hérite de NonPlayer
 * - Surcharge la méthode `updateBehavior()`
 * 
 * ## Fonctionnalités de Candle
 * 
 * 1. **Détection du joueur** (range = 500 pixels)
 *    - Si le joueur est détecté dans la zone, la chandelle le poursuit
 *    - Si le joueur s'échappe, la chandelle revient au repos
 * 
 * 2. **Poursuite horizontale**
 *    - Calcule la direction vers le joueur
 *    - Applique la vitesse dans cette direction
 *    - La gravité est gérée automatiquement par GameCharacter
 * 
 * 3. **Détection d'attaque** (range = 50 pixels)
 *    - Lorsque la chandelle est suffisamment proche, elle est en portée d'attaque
 *    - La logique d'attaque peut être implémentée ici
 * 
 * ## Exemple d'utilisation en jeu
 * 
 * ```cpp
 * // Création d'une chandelle
 * auto candleTexture = textureManager.getTexture("candle");
 * auto candle = std::make_unique<Candle>("Candle", 20, 0, 150.f, candleTexture);
 * candle->setPosition(300.f, 200.f);
 * 
 * // Dans la boucle de mise à jour du jeu
 * for (auto& enemy : enemies)
 * {
 *     enemy->updateBehavior(deltaTime, player, grounds);
 *     enemy->draw(window);
 * }
 * ```
 * 
 * ## Créer un nouvel ennemi IA
 * 
 * Pour créer un nouvel ennemi avec un comportement différent :
 * 
 * 1. Créer une classe qui hérite de NonPlayer
 * 2. Surcharger la méthode `updateBehavior()`
 * 3. Implémenter votre logique IA personnalisée
 * 
 * Exemple - Ennemi qui patrouille :
 * ```cpp
 * class Patrol : public NonPlayer {
 * private:
 *     sf::Vector2f patrolPointA, patrolPointB;
 * public:
 *     Patrol(const std::string& name, int hp, int mana, float speed, std::shared_ptr<sf::Texture> texture);
 *     void updateBehavior(float deltaTime, Player* player, const std::vector<std::unique_ptr<Ground>>& grounds) override;
 * };
 * ```
 * 
 * ## Points d'amélioration futurs
 * 
 * - Ajouter des états d'IA (IDLE, CHASE, ATTACK, FLEE)
 * - Implémenter un système de pathfinding pour des mouvement plus intelligents
 * - Ajouter de la variété dans les comportements
 * - Implémenter des attaques
 * - Ajouter une détection verticale pour les ennemis volants
 * - Système de groupe/coopération entre ennemis
 */
