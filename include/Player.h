#pragma once

#include <iostream>
#include "TileMap.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Image Index
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PlayerImgIdx {
static const short RUN_IDX_ARR_SIZE = 3;
static const short STAND = 0;
static const short DYING = 6;
static const short RUN[RUN_IDX_ARR_SIZE] = {1, 2, 3};
static const short JUMP = 5;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : private Entity {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Player Image Size in Pixel
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static const short PLAYER_WIDTH = 16;
    static const short PLAYER_HEIGHT_SMALL = 16;
    static const short PLAYER_HEIGHT_BIG = 32;
    
private:
    static inline const std::string PLAYER_IMG_PATH = "Graphics/Mario.png";
    static Player *m_pInstance;
    TileMap m_Map;
    
    sf::Vector2f m_TileVector;
    sf::View m_PlayerView;
    
    short m_PlayerImgIdx;
    short m_PlayerMoveIdx;
    short m_JumpFactor;

private:
    Player();
    ~Player();
    
public:
    static Player *GetInstance();
    static void ReleaseInstance();
    
    int LoadPlayerImage(sf::RenderWindow &winMario);
    void CheckPlayerState(const int frameX);
    bool IsCollision(const int frameX);
    void LandPlayer();
    void Move(Direction_e direction, int &frameX);
    
    inline void SetPlayerImageIdx(const short playerImgIdx) {
        m_PlayerImgIdx = playerImgIdx;
    }
    
    inline void RestePlayerMoveIdx() {
        m_PlayerMoveIdx = 0;
    }
    
    inline void SetPlayerStandInAir() {
        
    }
    
    friend class Mario;
};
