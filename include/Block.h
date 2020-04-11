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
    sf::Vector2f m_TileVector;
    
    short m_BlockIdxX;

public:
    Block();
    ~Block();
    
    int LoadBlockImage(sf::RenderWindow &m_WinMario, const int frameX, Obstacle::ObstacleBlock_s *const block);
};
