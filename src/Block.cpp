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
int Block::LoadBlockImage(sf::RenderWindow &m_WinMario, const short frameX, const short row, const short col, Obstacle::Behaviour_e blockType) {
    const int blockX = (col << BLOCK_SIZE_BIT) - frameX;                /// col  * 16 - Frame    : As Frame moves left then we need to check block with that offset rather than 0
    const int blockY = (row << BLOCK_SIZE_BIT) + (BLOCK_SIZE >> 1);     /// row * 16 + 8           : Shift 8  for last half ground block
    
    TileMap::PrintControl_s printControl;
    Entity::Position_s blockPosition(blockX, blockY);
    
    m_BlockIdxX = 0;
    
    Obstacle *pObstacle = Obstacle::GetInstance();
    
    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.position = blockPosition;
    printControl.imgY = blockType << BLOCK_SIZE_BIT;
    printControl.tile = m_BlockIdxX;
    printControl.tileSize = sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE);
    printControl.bInverted = false;
    
    if (!m_Tile.Load(printControl)) {
        LogError (BIT_BLOCK, "Block::LoadBlockImage() : Can Not Load Block Image \n");
        return EXIT_FAILURE;
    }
    
    m_BlockView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
    m_WinMario.setView(m_BlockView);
    m_WinMario.draw(m_Tile);

    return EXIT_SUCCESS;
}
