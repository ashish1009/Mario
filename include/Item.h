#pragma once

#include "Entity.h"
#include "Obstacle.h"

class Item : public Entity {
private:
    short m_BrickPartNum; /// 0 means top right part, 1, top left, 2, down rigjt, 3 down left
    short m_BrickPartMovePathIdx; /// path followed by brick parts
    
    float m_CoinJumpIdx;
    Block *m_pBlock;
    
public:
    int LoadItemImage(sf::RenderWindow &winMario);
    
    void DrawCoinJump(short &imgY);
    void DrawBonus(short &imgY);
    void DrawBrokenBrick(TileMap::PrintControl_s &printControl);
    
    inline void SetBrickPaertNum(const short brickPartNum) {
        m_BrickPartNum = brickPartNum;
    }
    inline void SetBlock(Block *block) {
        m_pBlock = block;
    }
    inline Block *GetBlock() const{
        return m_pBlock;
    }
    inline short GetBrickPartNum() const {
        return m_BrickPartNum;
    }
    
    Item();
    ~Item();
};
