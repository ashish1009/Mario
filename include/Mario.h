#pragma once

#include <iostream>
#include "Item.h"
#include "Block.h"
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Mario Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Mario {
private:
    sf::RenderWindow m_WinMario;
    sf::Vector2u m_WinSize;
    sf::Event m_MarioEvent;
    sf::Texture m_MarioTexture;
    sf::Sprite m_MarioSprite;
    sf::View m_MarioView;
    sf::Font m_Font;
    sf::Text m_Text;
    sf::Clock m_Clock;
    
    sf::Texture m_CoinScoreTexture;
    sf::Sprite m_CoinScoreSprite;
    
    Player *m_pPlayer;
    Obstacle *m_pObstacle;
    std::list<Item> m_pItem;
    
    Block m_Block;
    
    int m_FramePosition;
    int m_Score;
    short m_CoinCount;
    short m_Level;
    short m_Time;
    
private:    
    void DrawView();
    void DrawBlocks();
    void PrintContent();
    void PrintText(const char *printStuff, short variable, const float xPosition);
    
public:
    void PlayGame();
    
    Mario();
    ~Mario();
};
