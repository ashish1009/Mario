#pragma once

#include <iostream>
#include "Fire.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Image Index
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PlayerImgIdx {
static const short RUN_IDX_ARR_SIZE = 3;
static const short STAND = 0;
static const short DYING = 6;
static const short RUN[RUN_IDX_ARR_SIZE] = {1, 2, 3};
static const short JUMP = 5;
static const short MID_INC = 15;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : public Entity {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Player Image Size in Pixel
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static const short PLAYER_WIDTH = 16;
    static const short PLAYER_HEIGHT_SMALL = 16;
    static const short PLAYER_HEIGHT_BIG = 32;
    static const short MAX_JUMP_HEIGHT = 64;
    
private:
    static inline const std::string PLAYER_IMG_PATH = "Graphics/Mario.png";
    static Player *m_pInstance;
    TileMap m_Map;
    
    sf::Vector2f m_TileVector;
    sf::View m_PlayerView;
    
    Obstacle *m_pObstacle;
    
    short m_PlayerHeight;
    short m_PlayerImgIdx;
    short m_PlayerMoveIdx;
    short m_JumpFactor;

private:
    Player();
    ~Player();
    
public:
    static Player *GetInstance();
    static Player *ReleaseInstance();
    
    int LoadPlayerImage(sf::RenderWindow &winMario);
    void CheckPlayerState(const int frameX, sf::RenderWindow &winMario);
    void Jump(const int frameX);
    void Move(Entity::Direction_e direction, int &frameX, sf::RenderWindow &winMario);\
    void IncreaseSize();
    void IncJumFactor();
    void Fire();

    inline void SetImgIdx(const short playerImgIdx) {
        m_PlayerImgIdx = playerImgIdx;
    }
    
    inline void ResetMoveImgIdx() {
        m_PlayerMoveIdx = 0;
    }
    
    inline void ResetJumpFactor() {
        m_JumpFactor = 0;
    }
    
    inline int GetPlayerHeight() const {
        return m_PlayerHeight;
    }
    
    /// Pure virtual funxtion
    bool IsDownCollision (const int frameX) override;
    bool IsJumpCollision (const int frameX) override;
    bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) override;
    bool IsPlayerCollision () override;
    
    friend class Mario;
};
