#pragma once

#include <iostream>
#include "TileMap.h"

class Fire : public Entity{
private:
    TileMap m_Map;
    
public:
    Fire();
    ~Fire();
    
//    int LoadBlockImage(sf::RenderWindow &m_WinMario, short frameX, Obstacle::ObstacleBlock_s *block);
};
