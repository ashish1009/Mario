#pragma once
#include "WelcomeScreen.h"
#include "Obstacle.h"
#include "Player.h"
#include "Item.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Mario Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Mario : private WelcomeScreen, SoundBuffer {
private:
    sf::Music m_Music;
    sf::Clock m_Clock;
    
    short m_Score;
    short m_CoinCoint;
    short m_TimeLeft;
    
    float m_FrameShiftX;
    
    std::list<Item> m_lItem;
    std::list<Bullet> m_lBullet;
    
    Obstacle *m_pObstacle;
    Player *m_pPlayer;
    
private:
    void PolEvent() override;
    
    void StartScreen();
    void DrawBlocks();
    void DrawItems();
    void DrawBullets();
    
    inline void DrawPlayer() {
        m_pPlayer->LoadImage(m_Window);
    }
    
public:
    void PlayGame();
    
    Mario();
    ~Mario();
};
