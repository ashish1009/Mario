#pragma once

#include <iostream>
#include "Entity.h"
#include "TileMap.h"
#include "Obstacle.h"
#include "Common.h"

/*
 * Player Image Size in Pixel
 */
static const unsigned short PLAYER_WIDTH = 32;
static const unsigned short PLAYER_HEIGHT_SMALL = 32;
static const unsigned short PLAYER_HEIGHT_BIG = 64;

/*
 * Player Stariing Position in respect of Window Pixel
 */
static const unsigned short PLAYER_BASE_X = 40;
static const unsigned short PLAYER_BASE_Y = GROUND_LEVEL;

/*
 * Player Image Index
 */
namespace PlayerImgIdx
    {
    static const unsigned short RUN_IDX_ARR_SIZE = 3;
    static const unsigned short STAND = 0;
    static const unsigned short DYING = 6;
    static const unsigned short RUN[RUN_IDX_ARR_SIZE] = {1, 2, 3};
    static const unsigned short JUMP = 5;
    };

class Player : public Entity
{
private:
    static inline const std::string PLAYER_IMG_PATH = "Graphics/Mario.png";
    static const unsigned short DEFAULT_JUMP_HEIGHT = 80;
    static const unsigned short JUMP_FACTOR = 4;
    
    static Player *m_pInstance;
    sf::Vector2<float> m_Vector;
    sf::Vector2<float> m_PrintSize;
    TileMap m_Map;
    Obstacle *m_pObstacle;
    unsigned short m_PlayerImg;
    unsigned short m_JumpHeight;

private:
    void Init();
    void IncSize();
    bool IsCollision();
    
    Player();
    ~Player();

public:
    static Player *GetInstance();
    static void ReleaseInstance();

    int LoadPlayerImage();
    void DrawPlayer(sf::RenderWindow &m_WinMario) const;
    void Move(float &FrameX, const Player::Direction_e Direction);
    void SetNewPosition();
    void JumpPlayer();

    inline void SetPlayerImg(const unsigned short ImgIdx)
    {
        m_PlayerImg = ImgIdx;
    }
};
