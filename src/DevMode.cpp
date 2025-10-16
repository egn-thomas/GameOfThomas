#include "./DevMode.hpp"
#include <sstream>
#include <iostream>
#include <array>

DevMode::DevMode(bool active) : active(active)
{
    if (!font.loadFromFile("../src/assets/fonts/RobotoMono-Regular.ttf"))
    {
        std::cerr << "Impossible de charger la police pour DevMode\n";
    }

    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(10.f, 10.f);
}

void DevMode::drawInfo(sf::RenderWindow &window, const GameCharacter &player)
{
    if (!active)
        return;

    std::stringstream ss;
    ss << "Position: " << formatVector(player.getPosition()) << "\n";
    ss << "Velocity: " << formatVector(player.getVelocity()) << "\n";
    ss << "On Ground: " << (player.isOnGround() ? "YES" : "NO") << "\n";
    ss << "HP: " << player.getHp() << " / " << player.getMaxHp() << "\n";
    ss << "Mana: " << player.getMana() << " / " << player.getMaxMana() << "\n";

    std::array<bool, 4> contacts = player.getContacts();
    std::vector<std::string> labels = {"Top", "Bottom", "Left", "Right"};

    for (size_t i = 0; i < contacts.size(); ++i)
    {
        std::ostringstream oss;
        oss << "Contact " << labels[i] << " : " << (contacts[i] ? "true" : "false");
        text.setString(oss.str());
    }

    text.setString(ss.str());
    window.draw(text);
}

void DevMode::setActive(bool a) { active = a; }
bool DevMode::isActive() const { return active; }

std::string DevMode::formatVector(const sf::Vector2f &v) const
{
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ")";
    return ss.str();
}