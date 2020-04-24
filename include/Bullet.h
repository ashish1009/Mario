#pragma once

#include "Entity.h"

class Bullet : public Entity {
private:
    float m_BulletY;
    float m_BulletX;
    short m_ExplosionCounter;                       /// Counter to keep Explosion image

public:
    Bullet();
    ~Bullet();
    
    int LoadBulletImage(sf::RenderWindow &m_WinMario);
};
