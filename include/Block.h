#pragma once

#include <iostream>
#include "TileMap.h"
#include "Common.h"
#include <vector>

class Block
{
private:
    TileMap m_Tile;
    sf::View m_BlockView;
    
    short m_BlockIdxX;

public:
    Block();
    ~Block();
    
    int LoadBlockImage(sf::RenderWindow &m_WinMario, const short frameX, const short i, const short j, Obstacle::ObstacleBlock_s *block);
};
