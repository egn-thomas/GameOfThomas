#include "InventoryMenu.hpp"
#include <iostream>
#include <cmath>

InventoryMenu::InventoryMenu() {
    if (!font.loadFromFile("../src/assets/fonts/RobotoMono-Regular.ttf")) {
        std::cerr << "Erreur lors du chargement de la police pour l'inventaire" << std::endl;
    }
}

std::shared_ptr<sf::Texture> InventoryMenu::getTexture(const std::string& path) {
    auto it = textureCache.find(path);
    if (it != textureCache.end()) return it->second;

    // Try several common prefixes
    std::vector<std::string> tries = {path, "../src/" + path, "src/" + path, "../" + path};
    for (const auto& p : tries) {
        auto tex = std::make_shared<sf::Texture>();
        if (tex->loadFromFile(p)) {
            textureCache[path] = tex;
            return tex;
        }
    }
    // Debug: afficher les chemins essayés
    std::cerr << "[InventoryMenu] Failed to load image '" << path << "'. Tried paths:";
    for (const auto& p : tries) std::cerr << " '" << p << "'";
    std::cerr << std::endl;
    return nullptr;
}

sf::ConvexShape InventoryMenu::createRoundedRect(sf::Vector2f size, float radius, int cornerSegments) {
    if (radius <= 0.f) {
        sf::ConvexShape r;
        r.setPointCount(4);
        r.setPoint(0, {0.f, 0.f});
        r.setPoint(1, {size.x, 0.f});
        r.setPoint(2, {size.x, size.y});
        r.setPoint(3, {0.f, size.y});
        return r;
    }

    int segments = std::max(1, cornerSegments);
    sf::ConvexShape shape;
    shape.setPointCount(segments * 4);

    auto addArc = [&](float cx, float cy, float startAngle) {
        for (int s = 0; s < segments; ++s) {
            float t = static_cast<float>(s) / static_cast<float>(segments - 1);
            float angle = startAngle + t * (M_PI_2);
            float x = cx + std::cos(angle) * radius;
            float y = cy + std::sin(angle) * radius;
            shape.setPoint((int)shape.getPointCount() - 1, {x, y});
        }
    };

    // We'll compute points manually
    int idx = 0;
    for (int corner = 0; corner < 4; ++corner) {
        float cx = (corner == 0 || corner == 3) ? radius : size.x - radius;
        float cy = (corner == 0 || corner == 1) ? radius : size.y - radius;
        float startAngle = 0.f;
        switch (corner) {
            case 0: startAngle = M_PI; break;            // top-left: PI -> 3PI/2
            case 1: startAngle = 3.f * M_PI_2; break;   // top-right: 3PI/2 -> 2PI
            case 2: startAngle = 0.f; break;            // bottom-right: 0 -> PI/2
            case 3: startAngle = M_PI_2; break;         // bottom-left: PI/2 -> PI
        }
        for (int s = 0; s < segments; ++s) {
            float t = static_cast<float>(s) / static_cast<float>(segments - 1);
            float angle = startAngle + t * (M_PI_2);
            float x = cx + std::cos(angle) * radius;
            float y = cy + std::sin(angle) * radius;
            shape.setPoint(idx++, {x, y});
        }
    }

    return shape;
}

void InventoryMenu::draw(sf::RenderWindow& window, const GameCharacter& character) {
    // Fond semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    // Titre
    sf::Text invTitle;
    invTitle.setFont(font);
    invTitle.setString("INVENTAIRE");
    invTitle.setCharacterSize(60);
    invTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = invTitle.getLocalBounds();
    invTitle.setPosition(960.f - titleBounds.width / 2.f, 100.f);
    window.draw(invTitle);

    // Affichage des piles d'items
    const auto& inventory = character.getInventory();
    float startX = 300.f;
    float y = 300.f;
    float slotWidth = 120.f;
    float slotHeight = 120.f;
    float radius = 12.f;
    int slotCount = static_cast<int>(inventory.size());

    for (int i = 0; i < slotCount; ++i) {
        sf::Vector2f pos(startX + i * (slotWidth + 20.f), y);
        // Rounded rect background
        sf::ConvexShape bg = createRoundedRect({slotWidth, slotHeight}, radius, 8);
        bg.setPosition(pos);
        bg.setFillColor(unselectedColor);
        bg.setOutlineThickness(0.f);
        window.draw(bg);

        // If selected, draw a white outline rounded rect
        if (i == selectedSlot) {
            sf::ConvexShape outline = createRoundedRect({slotWidth, slotHeight}, radius, 8);
            outline.setPosition(pos);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineColor(sf::Color::White);
            outline.setOutlineThickness(3.f);
            window.draw(outline);
        }

        // Draw item sprite if present
        if (!inventory[i].stack.empty()) {
            const std::string& spritePath = inventory[i].stack.top()->getSpritePath();
            auto tex = getTexture(spritePath);
            if (tex) {
                sf::Sprite s;
                s.setTexture(*tex);
                // Scale to fit inside slot with some padding
                float padding = 12.f;
                float availW = slotWidth - 2.f * padding;
                float availH = slotHeight - 2.f * padding;
                float tw = static_cast<float>(tex->getSize().x);
                float th = static_cast<float>(tex->getSize().y);
                float scale = std::min(availW / tw, availH / th);
                s.setScale(scale, scale);
                float drawW = tw * scale;
                float drawH = th * scale;
                s.setPosition(pos.x + (slotWidth - drawW) / 2.f, pos.y + (slotHeight - drawH) / 2.f - 8.f);
                window.draw(s);
            }

            // Afficher le nombre
            sf::Text itemText;
            itemText.setFont(font);
            itemText.setCharacterSize(12);
            itemText.setString(inventory[i].itemTypeName + " x" + std::to_string((int)inventory[i].stack.size()));
            itemText.setFillColor(sf::Color::White);
            sf::FloatRect bounds = itemText.getLocalBounds();
            itemText.setPosition(pos.x + slotWidth / 2.f - bounds.width / 2.f, pos.y + slotHeight + 30.f);
            window.draw(itemText);
        } else {
            // Empty label
            sf::Text itemText;
            itemText.setFont(font);
            itemText.setCharacterSize(18);
            itemText.setString("Vide");
            itemText.setFillColor(sf::Color::White);
            sf::FloatRect bounds = itemText.getLocalBounds();
            itemText.setPosition(pos.x + slotWidth / 2.f - bounds.width / 2.f, pos.y + slotHeight / 2.f - bounds.height / 2.f);
            window.draw(itemText);
        }
    }
}

void InventoryMenu::moveSelection(int direction) {
    selectedSlot += direction;
    if (selectedSlot < 0) selectedSlot = 0;
    if (selectedSlot > 9) selectedSlot = 9;
}
