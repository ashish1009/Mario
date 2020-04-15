#pragma once

#include "TileMap.h"

class Bullet : public Entity {
private:
    sf::Vector2f m_TileVector;
    
    TileMap m_Map;
    short m_BulletY;
    short m_BulletX;
    short m_ExplosionCounter;                       /// Counter to keep Explosion image

public:
    Bullet();
    ~Bullet();
    
    int LoadItemImage(sf::RenderWindow &m_WinMario, short frameX);

    /// Pure Virtual Funxtion
    bool IsPlayerCollision() override;
    bool IsDownCollision (const int frameX) override;
    bool IsJumpCollision (const int frameX) override;
    bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) override;
};
