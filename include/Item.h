#pragma once

#include "TileMap.h"

class Item : public Entity, private SoundBuffer{
private:
    sf::Vector2f m_TileVector;
    
    TileMap m_Map;
    short m_IdxY;                               /// Relative position of Bonus after popped
    short m_IdxX;                               /// Relative Position of Bonus after popped
    int m_FramePos;

    Obstacle::ObstacleBlock_s *m_pBlock;
    sf::Vector2<short> m_ItemBreakPart;
    short m_ItemMovePathIdx;
    
    sf::Clock clock;
    
public:
    Item();
    ~Item();
    
    int LoadItemImage(sf::RenderWindow &m_WinMario, short frameX);
    
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
    
    inline void SetItemBreakPart(const short X, const short Y) {
        m_ItemBreakPart.x = X;
        m_ItemBreakPart.y = Y;
    }
    
    inline sf::Vector2<short> GetItemBreakPart() const {
        return m_ItemBreakPart;
    }

    /// Pure Virtual Funxtion
    bool IsPlayerCollision() override;
    bool IsDownCollision (const int frameX) override;
    bool IsJumpCollision (const int frameX) override;
    bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) override;
};
