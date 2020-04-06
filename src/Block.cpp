#include "Block.h"
#include "Logger.h"
#include "Obstacle.h"
#include <math.h>

Block::Block()
: m_BlockIdxX(0) {
}

Block::~Block() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Block Image and sst all pixel as obstacle
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Block::LoadBlockImage(sf::RenderWindow &winMario, const short frameX, const short row, const short col, Obstacle::ObstacleBlock_s *block) {
    block->xPos = (col << BLOCK_SIZE_BIT) - frameX;                /// col  * 16 - Frame    : As Frame moves left then we need to check block with that offset rather than 0
    block->yPos = (row << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1);     /// row * 16 + 8           : Shift 8  for last half ground block
    
    TileMap::PrintControl_s printControl;
    Entity::Position_s blockPosition(block->xPos, block->yPos);
    
    if (Obstacle::BONUS == block->behaviour) {
        if (block->bIsPopped && !block->bIsEmpty) {
            const int maxPopHeight = 8;                                     /// Maximum Height that block will move up when popped popped
            
            /// Pope the Block upto height  : 8
            if (maxPopHeight >= block->upPopped) {
                blockPosition.SetPositionLocal(blockPosition.X, blockPosition.Y - block->upPopped);
                block->upPopped++;
            }
            /// after poped it is set as empty block
            else {
                block->bIsEmpty = true;
                block->upPopped = 0;
            }
        } /// if (block->bIsPopped && !block->bIsEmpty)
        else {
            if (block->bIsEmpty) {
                /// as Empty Image is in Brick Row
                block->behaviour = Obstacle::BRICK;
            }
            else {
                /// Logic for Blinking Blocks
                const int blinkDelay = 7;                                       /// Divide by 8
                static short blinkCounter = 0;
                blinkCounter++;
                m_BlockIdxX = blinkCounter >> blinkDelay;
                m_BlockIdxX %= 4;                                               /// there are 4 images to blink
            }
        }
    } /// if (Obstacle::BONUS == block->behaviour)
    else {
        if (block->bIsEmpty) {
            /// Image of EMPTY block
            m_BlockIdxX = 2;
        }
        else {
            /// Default Bonus Image
            m_BlockIdxX = 0;
        }
    }
    
    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.position = blockPosition;
    printControl.imgY = block->behaviour << BLOCK_SIZE_BIT;
    printControl.tile = m_BlockIdxX;
    printControl.tileSize = sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE);
    printControl.bInverted = false;
    
    if (!m_Tile.Load(printControl)) {
        LogError (BIT_BLOCK, "Block::LoadBlockImage() : Can Not Load Block Image \n");
        return EXIT_FAILURE;
    }
    
    m_BlockView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
    winMario.setView(m_BlockView);
    winMario.draw(m_Tile);

    return EXIT_SUCCESS;
}
