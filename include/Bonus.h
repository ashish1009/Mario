#pragma once

#include <iostream>
#include "Obstacle.h"
#include "Player.h"

class Bonus : public Entity{
private:
    static Bonus* m_pInstance;
    
    TileMap m_Map;
    short m_IdxY;
    short m_IdxX;
    int m_FramePos;
    Obstacle::ObstacleBlock_s *m_pBlock;
    
private:
    Bonus();
    ~Bonus();
    
public:
    static Bonus *CreateInstance();
    static Bonus *ReleaseInstance();
    static Bonus *GetInstance();
    
    bool IsBonusCollion(Player *pPlayer);
    int LoadBlockImage(sf::RenderWindow &m_WinMario, short frameX, Obstacle::ObstacleBlock_s *block);
    
    inline void SetBlock(Obstacle::ObstacleBlock_s *blockType) {
        m_pBlock = blockType;
    }
    
    inline Obstacle::ObstacleBlock_s *getBlock() const {
        return m_pBlock;
    }
    
    inline int GetFramePos() const {
        return m_FramePos;
    }
    
    inline void SetFramePos(const int framePos) {
        m_FramePos = framePos;
    }
    
    inline void SetXPos(const short X) {
        m_IdxX = X;
    }

    /// Pure Virtual Funxtion
    bool IsDownCollision (const int frameX) override;
    bool IsJumpCollision (const int frameX) override;
    bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) override;

};
