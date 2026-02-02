#include "UIManager.hpp"
#include <algorithm>

void UIManager::addNotification(const std::string &text, float duration)
{
    notifications.push_back({text, duration});
}

void UIManager::updateNotifications(float deltaTime)
{
    for (auto &n : notifications)
        n.ttl -= deltaTime;
    notifications.erase(std::remove_if(notifications.begin(), notifications.end(), [](const Notification &n){ return n.ttl <= 0.f; }), notifications.end());
}

void UIManager::drawNotifications(sf::RenderWindow& window)
{
    if (!fontLoaded || notifications.empty())
        return;

    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    float x = window.getSize().x - notificationMargin - notificationWidth;
    float y = notificationMargin;
    float padding = 10.f;
    float lineHeight = 32.f;

    for (size_t i = 0; i < notifications.size(); ++i)
    {
        const Notification &n = notifications[i];
        sf::RectangleShape bg(sf::Vector2f(notificationWidth, lineHeight));
        bg.setPosition(x, y + i * (lineHeight + 8.f));
        bg.setFillColor(sf::Color(0, 0, 0, 150));
        bg.setOutlineColor(sf::Color::White);
        bg.setOutlineThickness(1.f);
        window.draw(bg);

        sf::Text text;
        text.setFont(font);
        text.setString(n.text);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + padding, y + i * (lineHeight + 8.f) + 4.f);
        window.draw(text);
    }

    window.setView(currentView);
}