#include "Chest.hpp"
#include "../items/HealthPotion.hpp"
#include "../items/ManaPotion.hpp"
#include "../items/HealthAmulet.hpp"
#include "../items/DamageAmulet.hpp"
#include <random>
#include <functional>
#include <algorithm>

/**
 * @brief Constructeur de Chest
 */
Chest::Chest(const sf::Vector2f &position, std::shared_ptr<sf::Texture> texture)
    : Object("Chest", position, texture), closedTexture(texture)
{
    // Scaling par défaut (4x comme les autres éléments du jeu)
    sprite.setScale(4.f, 4.f);
    
    // Charger la texture du coffre ouvert
    openedTexture = std::make_shared<sf::Texture>();
    if (!openedTexture->loadFromFile("../src/assets/images/chestOpened.png"))
    {
        // Si le fichier n'existe pas, utiliser la même texture
        openedTexture = closedTexture;
    }
}

/**
 * @brief Génère le contenu du coffre en fonction du niveau de jeu.
 * @param level Le niveau de jeu actuel
 */
std::vector<std::unique_ptr<Item>> Chest::generateLoot(int level) const
{
    // Nombre d'objets dépendant du niveau (1..N) : 1 pour 1-10, 2 pour 11-20, etc.
    int count = (level - 1) / 10 + 1;
    // Prévenir des coffres gigantesques : cap raisonnable
    if (count > 5) count = 5;

    // Moteur aléatoire (seed une fois)
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Plages dynamiques basées sur le niveau pour varier les effets
    std::uniform_int_distribution<int> hpPotRange(10 + level, 20 + level * 2);
    std::uniform_int_distribution<int> manaPotRange(5 + level / 2, 15 + level);
    std::uniform_int_distribution<int> healthAmuletRange(5 + level / 5, 15 + level / 2);
    std::uniform_int_distribution<int> damageAmuletRange(1 + level / 10, 3 + level / 20);

    // Table des loot : pair(poids, factory)
    std::vector<std::pair<int, std::function<std::unique_ptr<Item>()>>> table;
    table.push_back({40, [&](){ return std::make_unique<HealthPotion>(hpPotRange(gen)); }});
    table.push_back({25, [&](){ return std::make_unique<ManaPotion>(manaPotRange(gen)); }});
    table.push_back({15, [&](){ return std::make_unique<HealthAmulet>(healthAmuletRange(gen)); }});
    table.push_back({20, [&](){ return std::make_unique<DamageAmulet>(damageAmuletRange(gen)); }});

    int totalWeight = 0;
    for (auto &p : table) totalWeight += p.first;

    std::uniform_int_distribution<int> pickDist(1, totalWeight);

    std::vector<std::unique_ptr<Item>> loot;
    for (int i = 0; i < count; ++i)
    {
        int r = pickDist(gen);
        int accum = 0;
        for (auto &p : table)
        {
            accum += p.first;
            if (r <= accum)
            {
                loot.push_back(p.second());
                break;
            }
        }
    }

    return loot;
}

/**
 * @brief Vérifie si le coffre est ouvert
 */
bool Chest::getIsOpened() const
{
    return this->isOpened;
}

/**
 * @brief Ouvre le coffre
 */
void Chest::open()
{
    if (!this->isOpened)
    {
        this->isOpened = true;
        this->setTexture(true);  // Changer la texture pour l'état ouvert
    }
}

/**
 * @brief Définit la texture du coffre
 * @param opened true pour ouvert, false pour fermé
 */
void Chest::setTexture(bool opened)
{
    if (opened)
    {
        this->texture = openedTexture;
    }
    else
    {
        this->texture = closedTexture;
    }
    this->sprite.setTexture(*this->texture);
}

/**
 * @brief Vérifie si le joueur est sur le coffre (collision AABB)
 */
bool Chest::isPlayerOnChest(const sf::FloatRect &playerBounds) const
{
    return hitbox.intersects(playerBounds);
}
