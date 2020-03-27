#include "Block.h"
#include "Logger.h"
#include "Obstacle.h"
#include <math.h>

#define NUM_SCORE_BLOCK_BLINK 3
#define BLINK_DELAY 40

Block::Block()
: m_BlockIdxX(0) {
}

Block::~Block() {
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Block Image and sst all pixel as obstacle
///         m_WinMario get Updated
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Block::LoadBlockImage(sf::RenderWindow &m_WinMario, const short frameX, const short row, const short col) {
    float blockX = (col * BLOCK_SIZE) - frameX;
    float blockY = (row * BLOCK_SIZE) + (BLOCK_SIZE >> 1);  /// Shift for lasg half ground block
    
    TileMap::PrintControl_s printControl;
    Entity::Position_s blockPosition(blockX, blockY);
    
    m_BlockIdxX = 0;
    
    Obstacle *pObstacle = Obstacle::GetInstance();
    float tileSize = (BLOCK_SIZE == pObstacle->GetBlockSizeAt(row, col) ? BLOCK_SIZE : BLOCK_SIZE * 2); //  TODO;

    for (int i = blockPosition.Y - BLOCK_SIZE; i < blockPosition.Y; i++) {
        for (int j = blockPosition.X; j < blockPosition.X + tileSize; j++) {
            if (0 <= i) {
                pObstacle->SetObstacle(i, j, pObstacle->GetBlockTypeAt(row, col), pObstacle->GetBlockAbilityAt(row, col));
            }
        }
    }
    
    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.position = blockPosition;
    printControl.imgY = pObstacle->GetBlockTypeAt(row, col) * BLOCK_SIZE;
    printControl.tile = m_BlockIdxX;
    printControl.tileSize = sf::Vector2f(tileSize, BLOCK_SIZE);
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
