#include "./DevMode.hpp"
#include <sstream>
#include <iostream>
#include <array>

/**
 * @brief Construct a new Dev Mode:: Dev Mode object
 * 
 * @param active Indique si le mode développeur est actif au lancement.
 */
DevMode::DevMode(bool active) : active(active)
{
    if (!font.loadFromFile("../src/assets/fonts/RobotoMono-Regular.ttf"))
    {
        std::cerr << "Impossible de charger la police pour DevMode\n";
        fontLoaded = false;
    }
    else
    {
        fontLoaded = true;
        text.setFont(font);
    }
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Red);
    text.setPosition(10.f, 10.f);
}

/**
 * @brief Affiche les informations du joueur et de debug à l'écran.
 * 
 * @param window La fenêtre SFML où dessiner les informations.
 * @param player Le personnage joueur dont on affiche les infos.
 * @param allCharacters Tous les personnages du jeu pour afficher leurs HP.
 */
void DevMode::drawInfo(sf::RenderWindow &window, const GameCharacter &player, std::vector<GameCharacter *> allCharacters)
{
    if (!active)
        return;

    std::stringstream ss;
    ss << "Position: " << formatVector(player.getPosition()) << "\n";
    ss << "Velocity: " << formatVector(player.getVelocity()) << "\n";
    ss << "On Ground: " << (player.isOnGround() ? "YES" : "NO") << "\n";
    ss << "HP: " << player.getHp() << " / " << player.getMaxHp() << "\n";
    ss << "Mana: " << player.getMana() << " / " << player.getMaxMana() << "\n";
    for (auto character : allCharacters) {
        ss << character->getName() << " : " << character->getHp() << "\n";
    }    

    if (fontLoaded)
    {
        text.setString(ss.str());
        window.draw(text);
    }
}

void DevMode::drawDebugOverlays(sf::RenderWindow &window, const GameCharacter &player, const std::vector<std::unique_ptr<Ground>>& grounds, std::vector<GameCharacter *> allCharacters)
{
    if (!active) return;

    // draw grounds bounds outlines
    for (const auto &g : grounds)
    {
        sf::RectangleShape s = g->getShape();
        s.setFillColor(sf::Color::Transparent);
        s.setOutlineColor(sf::Color::Green);
        s.setOutlineThickness(1.f);
        window.draw(s);
    }

    // draw player hitbox
    sf::FloatRect pb = player.getBounds();
    sf::RectangleShape ph(sf::Vector2f(pb.width, pb.height));
    ph.setPosition(pb.left, pb.top);
    ph.setFillColor(sf::Color::Transparent);
    ph.setOutlineColor(sf::Color::Yellow);
    ph.setOutlineThickness(1.f);
    window.draw(ph);

    // draw other characters hitboxes
    for (auto c : allCharacters)
    {
        if (c == &player) continue;
        sf::FloatRect cb = c->getBounds();
        sf::RectangleShape ch(sf::Vector2f(cb.width, cb.height));
        ch.setPosition(cb.left, cb.top);
        ch.setFillColor(sf::Color::Transparent);
        ch.setOutlineColor(sf::Color::Magenta);
        ch.setOutlineThickness(1.f);
        window.draw(ch);
    }
}

/**
 * @brief Active ou désactive le mode développeur.
 */
void DevMode::setActive(bool a) { active = a; }

/**
 * @brief Indique si le mode développeur est actif.
 */
bool DevMode::isActive() const { return active; }

/**
 * @brief Formate un vecteur 2D en chaîne de caractères lisible.
 * 
 * @param v Le vecteur à formater.
 * @return std::string La représentation textuelle du vecteur.
 */
std::string DevMode::formatVector(const sf::Vector2f &v) const
{
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ")";
    return ss.str();
}