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
    sf::Clock m_StartScreenClock;
    sf::Clock m_Clock;
    
    bool m_bPause;
    
    short m_Score;
    short m_CoinCoint;
    short m_TimeLeft;
    
    float m_FrameShiftX;
    
    std::list<Item> m_lItem;
    std::list<Enemy> m_lEnemy;
    
    Obstacle *m_pObstacle;
    Player *m_pPlayer;
    
private:
    void AddEnemy();
    void PolEvent() override;
    
    void StartScreen();
    void DrawBlocks();
    void DrawItems();
    void DrawEnemy();
    void CheckPlayerKill();
    void PausePlayerLoop(const short imgX, const short imgY, TileMap &map);
    void PausePlayer();
    
    inline void DrawPlayer() {
        m_pPlayer->LoadImage(m_Window);
    }
    
public:
    void PlayGame();
    
    Mario();
    ~Mario();
};
