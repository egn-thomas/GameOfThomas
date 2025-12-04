#include "Ground.hpp"

/**
 * @brief Constructeur de la classe Ground.
 * 
 * @param x La position en x du sol.
 * @param y La position en y du sol.
 * @param width La largeur du sol.
 * @param height La hauteur du sol.
 * 
 * @note Le sol est représenté par un rectangle vert.
 */
Ground::Ground(float x, float y, float width, float height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(x, y);
}

/**
 * @brief Dessine le sol dans la fenêtre donnée.
 */
void Ground::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

/**
 * @brief Retourne les limites du sol sous forme de sf::FloatRect.
 */
const sf::FloatRect Ground::getBounds() const {
    return shape.getGlobalBounds();
}

/**
 * @brief Retourne la forme du sol.
 */
sf::RectangleShape Ground::getShape() {
    return shape;
}

/**
 * @brief Retourne si le sol est solide ou non.
 */
bool Ground::isGroundSolid() const {
    return isSolid;
}
