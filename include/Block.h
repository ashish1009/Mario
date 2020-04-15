#pragma once

#include "TileMap.h"

class Block : private SoundBuffer
{
private:
    TileMap m_Tile;
    sf::View m_BlockView;
    sf::Vector2f m_TileVector;
    
    short m_BlockIdxX;

public:
    Block();
    ~Block();
    
    int LoadBlockImage(sf::RenderWindow &m_WinMario, const int frameX, Obstacle::ObstacleBlock_s *const block, Entity::Size_e playerSize);
};
