#include "Block.h"
#include "Logger.h"
#include "Obstacle.h"
#include <math.h>

Block::Block()
: m_TileVector(BLOCK_SIZE, BLOCK_SIZE), m_BlockIdxX(0) {
}

Block::~Block() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Block Image and sst all pixel as obstacle
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Block::LoadBlockImage(sf::RenderWindow &winMario, Obstacle::ObstacleBlock_s *const blockObst) {
    TileMap::PrintControl_s printControl;
    Entity::Position_s blockPosition(blockObst->xPos, blockObst->yPos);
    
    if (Obstacle::BONUS == blockObst->behaviour) {
        if (blockObst->bIsEmpty) {
            blockObst->behaviour = Obstacle::BRICK;                             /// as Empty Image is in Brick Row
        } /// if (blockObst->bIsEmpty)
        else {
            if (blockObst->bIsPopped) {
                const int maxPopHeight = 8;                                     /// Maximum Height that block will move up when popped popped
                /// Pop the Block upto height  : 8
                if (maxPopHeight >= blockObst->upPopped) {
                    blockObst->upPopped++;
                    blockPosition.SetPositionLocal(blockPosition.X, blockPosition.Y - blockObst->upPopped);
                }
                /// after poped it is set as empty block
                else {
                    blockObst->bIsEmpty = true;
                    blockObst->upPopped = 0;
                }
            } /// if (block->bIsPopped && !block->bIsEmpty)
            else {
                /// Logic for Blinking Blocks
                const int blinkDelay = 7;                                       /// Divide by 8
                static short blinkCounter = 0;
                m_BlockIdxX = (blinkCounter++) >> blinkDelay;
                m_BlockIdxX %= 4;                                               /// there are 4 images to blink
            }
        }
    } /// if (Obstacle::BONUS == block->behaviour)
    else {
        if (blockObst->bIsEmpty) {
            m_BlockIdxX = 2;                                                    /// Image of EMPTY block
        }
        else {
            m_BlockIdxX = 0;                                                    /// Default Bonus/Brick Image
        }
    }
    
    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.position = blockPosition;
    printControl.imgY = blockObst->behaviour << BLOCK_SIZE_BIT;
    printControl.tile = m_BlockIdxX;
    printControl.tileSize = m_TileVector;
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
