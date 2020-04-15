#include "Block.h"
#include "Logger.h"
#include "Obstacle.h"
#include <math.h>

const int blinkDelay = 7;                                       /// Divide by 8
const int maxPopHeight = 8;                                     /// Maximum Height that block will move up when popped popped

Block::Block()
: m_TileVector(BLOCK_SIZE, BLOCK_SIZE), m_BlockIdxX(0) {
}

Block::~Block() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Block Image and sst all pixel as obstacle
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Block::LoadBlockImage(sf::RenderWindow &winMario, const int frameX, Obstacle::ObstacleBlock_s *const blockObst, Entity::Size_e playerSize) {
    Entity::Position_s blockPosition(blockObst->xPos, blockObst->yPos);
    m_BlockIdxX = 0;                                                        /// Default Bonus/Brick Image
    
    if (blockObst->bIsEmpty) {
        blockObst->behaviour = Obstacle::BRICK;                             /// as Empty Image is in Brick Row
        Obstacle *pObstacle = Obstacle::GetInstance();
        if (!pObstacle) {
            LogError(BIT_BONUS, "Bonus::LoadBlockImage(), Obstacle Istance is nULL \n");
            return EXIT_FAILURE;
        }
        
        /// Brivk Broken
        if (Obstacle::NO_ABILITY == blockObst->abilities) {
            const int blockRow = blockObst->yPos >> BLOCK_SIZE_BIT;
            const int blockCol = (frameX >> BLOCK_SIZE_BIT) + (blockObst->xPos >> BLOCK_SIZE_BIT) + 1;
            pObstacle->ResetBlockPixels(blockRow, blockCol);
            return EXIT_SUCCESS;
        }
        else {
            m_BlockIdxX = 2;                                                /// Image of EMPTY block
        }
    } /// if (blockObst->bIsEmpty)
    else {
        if (blockObst->bIsPopped) {
            /// Pop the Block upto height  : 8
            if (maxPopHeight >= blockObst->upPopped++) {
                if(1 == blockObst->upPopped) {
                    if (Obstacle::COIN == blockObst->abilities) {
                        m_Sound.setBuffer(m_CoinSound);
                        m_Sound.play();
                    }
                    else if ((Obstacle::NO_ABILITY == blockObst->abilities) && (Entity::BIG == playerSize)){
                        m_Sound.setBuffer(m_BrickSmashSound);
                        m_Sound.play();
                    }
                    else if (Obstacle::MUSHROOM == blockObst->abilities) {
                        m_Sound.setBuffer(m_PowerUpSound);
                        m_Sound.play();
                    }
                }
                blockPosition.SetPositionLocal(blockPosition.X, blockPosition.Y - blockObst->upPopped);
            }
            /// after poped it is set as empty block
            else {
                blockObst->upPopped = 0;
                const Obstacle::Behaviour_e &blockbehav = blockObst->behaviour;
                if ((Obstacle::BONUS == blockbehav) || (Obstacle::BRICK == blockbehav && Entity::BIG == playerSize)) {
                    blockObst->bIsEmpty = true;
                }
                else {
                    blockObst->bIsPopped = false;
                }
            }
        } /// if (block->bIsPopped && !block->bIsEmpty)
        else {
            if (Obstacle::BONUS == blockObst->behaviour) {
                /// Logic for Blinking Blocks
                static short blinkCounter = 0;
                m_BlockIdxX = (blinkCounter++) >> blinkDelay;
                m_BlockIdxX %= 4;                                               /// there are 4 images to blink
            }
        }
    }
    
    TileMap::PrintControl_s printControl;
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
