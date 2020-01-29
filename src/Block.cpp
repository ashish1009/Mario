#include "Block.h"
#include "Logger.h"
#include "Obstacle.h"

Block::Block()
: m_BlockImg(0), m_Vector(SQUARE_BLOCK_SIZE, SQUARE_BLOCK_SIZE)
{
    LogInfo(BIT_BLOCK, "Block::Block(), Constructor !!!!! \n");
}

Block::~Block()
{
    LogInfo(BIT_BLOCK, "Block::~Block(), Destructor !!!!! \n");
}

int Block::LoadBlockImage(Entity::Position_s Position)
{
#define BLOCK_PRINT_SIZE 64
    TileMap::PrintControl_s PrintControl;
    
    {
#define NUM_SCORE_BLOCK_BLINK 3
        static short BlockImgIdx = 0;
        BlockImgIdx++;
        
        m_BlockImg = (BlockImgIdx / 10) % NUM_SCORE_BLOCK_BLINK;
    }
    
    {
        Obstacle *pObstacle = Obstacle::GetInstance();

        short X = (short)(BLOCK_PRINT_SIZE * WIN_WIDTH) / 1000;
        short Y = (short)(BLOCK_PRINT_SIZE * WIN_HEIGHT) / 1000;

        for (short i = Position.X; i <= (Position.X + X); i++)
        {
            for (short j = (Position.Y - Y); j <= Position.Y; j++)
            {
                pObstacle->SetObstacle(i, j, Obstacle::COIN, Obstacle::BONUS);
            }
        }
    }
    
    PrintControl.TileSet = ResourcePath + BLOCK_IMG_PATH;
    PrintControl.Position = Position;
    PrintControl.ImgY = 5 * SQUARE_BLOCK_SIZE; //  TODO : chnage dynamicllly for other obstacles
    PrintControl.Tile = m_BlockImg;
    PrintControl.TileSize = m_Vector;
    PrintControl.PrintSize = {BLOCK_PRINT_SIZE, BLOCK_PRINT_SIZE};
    PrintControl.bInverted = false;

    if (!m_Tile.Load(PrintControl))
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

void Block::DrawBlock(sf::RenderWindow &m_WinMario) const
{
    sf::View view2;
    m_WinMario.setView(view2);
    m_WinMario.draw(m_Tile);
}


// 100 - 125
// 136 - 150
