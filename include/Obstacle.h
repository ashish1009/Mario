#pragma once
#include <array>
#include "Common.h"
#include "Block.h"
#include "SoundBuffer.h"

const short gRow9ForBlock = 9;
const short gRow5ForBlock = 5;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Obstacle Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Obstacle : public SoundBuffer {
private:
    static constexpr short NUM_ROW_PIXEL = NUM_ROW << BLOCK_SIZE_BIT;
    static constexpr short NUM_COL_PIXEL = NUM_COL << BLOCK_SIZE_BIT;
    
    static Obstacle *m_Instance;

    ///Row' refer to position Y and column refer to position X
    std::array<std::array<bool, NUM_COL_PIXEL>, NUM_ROW_PIXEL> m_Obstacle;
    std::array<std::array<Block, NUM_COL>, NUM_ROW> m_Block;
    
    TileMap m_Map;
    
private:
    Obstacle();
    void InitObstacle();
    void SetBlocks(Block &block);
    
    void SetBlockPixels(const Block &block);
    
public:
    inline bool GetObstacle(const int row, const int col) const {
        return m_Obstacle[row][col];
    }
    
    inline Block *GetBlock(const int row, const int col) {
        return &m_Block[row][col];
    }
    
    void SetPoppedBlock(const int row, const int col, const bool bBigPlayer = false);
    void LoadBlockImage(sf::RenderWindow &window, const int row, const int col);
    void ResetBlockPixel(Block &block);
    void EmptyBlock(Block &block, short &imgIdxX);
    void PopBlock(Block &block);
    void BlinkBlock(Block &block, short &imgIdxX);
    static Obstacle *GetInstance();
    static Obstacle *ReleaseInstance();
    ~Obstacle ();
};
