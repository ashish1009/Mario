#pragma oonce

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

class Graphics
{
protected:
    sf::Texture m_Texture;
    sf::Sprite m_Sprite;
    sf::View m_View;
    sf::Font m_Font;
    sf::Text m_Text;
    sf::Clock m_Clock;

};
