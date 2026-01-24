#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseMenu
{
public:
    enum class MenuOption
    {
        Continue,
        Quit,
        Count
    };

    PauseMenu();
    
    void draw(sf::RenderWindow& window);
    void moveSelection(int direction);
    MenuOption getSelectedOption() const { return selectedOption; }
    void resetSelection() { selectedOption = MenuOption::Continue; }
    
private:
    MenuOption selectedOption;
    sf::Font font;
    std::vector<sf::Text> menuItems;
    
    void initializeMenu();
    sf::Color selectedColor = sf::Color::Yellow;
    sf::Color unselectedColor = sf::Color::White;
};
