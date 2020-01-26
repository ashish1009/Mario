#pragma once

#include <iostream>
#include "TileMap.h"

class Block
{
private:
    static inline const std::string BLOCK_IMG_PATH = "Graphics/Block.png";
    static const unsigned short SQUARE_BLOCK_SIZE = 16;

    TileMap m_Tile;
    unsigned short m_BlockImg;
    sf::Vector2<float> m_Vector;

public:
    Block();
    ~Block();
    
    int LoadBlockImage(Entity::Position_s Position);
    void DrawBlock(sf::RenderWindow &m_WinMario) const;
    
};
