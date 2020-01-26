#pragma once

#include <iostream>
#include "Player.h"
#include "Block.h"
#include "Graphics.h"

class Mario : public Graphics
{
private:
    static const unsigned short MARIO_HEIGHT = 224;
    static const unsigned short MARIO_WIDTH = 384;
    static const unsigned short TIME_LIMIT = 400;
    
private:
    sf::RenderWindow m_WinMario;
    sf::Event m_MarioEvent;
    sf::Texture m_CoinScoreTexture;
    sf::Sprite m_CoinScoreSprite;
    
    Player *m_pPlayer;
    Obstacle *m_pObstacle;
    Block m_Block;
    
    float m_FramePosition;
    unsigned int m_Score;
    unsigned short m_CoinCount;
    unsigned short m_Level;
    unsigned short m_Time;
    
private:
    void MoveBgAt();
    void DrawPlayer();
    void DrawBlock();
    void PrintContent();
    void PrintText(const char *PrintStuff, unsigned short Variable, const float Xposition);
    void ResetScreenAndPlayer();
    
    void MovePlayer(Entity::Direction_e Direction)
    {
        m_pPlayer->Move(m_FramePosition, Direction);
    }

public:
    int PlayGame();

    Mario();
    ~Mario();
};
