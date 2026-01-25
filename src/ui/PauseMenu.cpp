#include "PauseMenu.hpp"
#include <iostream>

PauseMenu::PauseMenu() : selectedOption(MenuOption::Continue)
{
    initializeMenu();
}

void PauseMenu::initializeMenu()
{
    // Charger la police
    if (!font.loadFromFile("../src/assets/fonts/RobotoMono-Regular.ttf"))
    {
        std::cerr << "Erreur lors du chargement de la police pour le menu de pause" << std::endl;
        // On continue quand même avec une police par défaut
    }

    // Créer les éléments du menu
    std::vector<std::string> options = {"Continuer", "Quitter"};
    
    float startY = 250.f;
    float spacing = 100.f;
    
    for (size_t i = 0; i < options.size(); ++i)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(50);
        text.setPosition(400.f, startY + i * spacing);
        
        // Centrer le texte
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(960.f - bounds.width / 2.f, startY + i * spacing);
        
        menuItems.push_back(text);
    }
    
    // Colorer la première option comme sélectionnée
    if (!menuItems.empty())
        menuItems[0].setFillColor(selectedColor);
}

void PauseMenu::draw(sf::RenderWindow& window)
{
    // std::cout << "[DEBUG PAUSEMENU] draw() appelé" << std::endl;
    // Dessiner un fond semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    
    // Dessiner le titre
    sf::Text pauseTitle;
    pauseTitle.setFont(font);
    pauseTitle.setString("PAUSE");
    pauseTitle.setCharacterSize(80);
    pauseTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = pauseTitle.getLocalBounds();
    pauseTitle.setPosition(960.f - titleBounds.width / 2.f, 100.f);
    window.draw(pauseTitle);
    
    // Dessiner les options du menu
    for (auto& item : menuItems)
    {
        window.draw(item);
    }
}

void PauseMenu::moveSelection(int direction)
{
    // Désélectionner l'option actuelle
    int currentIndex = static_cast<int>(selectedOption);
    menuItems[currentIndex].setFillColor(unselectedColor);
    
    // Mettre à jour la sélection
    int newIndex = currentIndex + direction;
    if (newIndex < 0)
        newIndex = static_cast<int>(MenuOption::Count) - 1;
    else if (newIndex >= static_cast<int>(MenuOption::Count))
        newIndex = 0;
    
    selectedOption = static_cast<MenuOption>(newIndex);
    menuItems[newIndex].setFillColor(selectedColor);
}
