#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
class GameCharacter; // Forward declaration

class Item {
public:
    virtual ~Item() = default;
    // Applique l'effet de l'item au personnage
    virtual void applyEffect(GameCharacter& character) = 0;
    // Nom de l'item
    virtual std::string getName() const = 0;
    // Description de l'item
    virtual std::string getDescription() const = 0;
    // Chemin du sprite
    virtual std::string getSpritePath() const = 0;
};

#endif // ITEM_HPP
