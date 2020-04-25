#pragma once

#include "Entity.h"

class Bullet : public Entity {
private:
    Direction_e m_Direction;
    
    float m_BulletY;
    float m_BulletX;
    short m_ExplosionCounter;                       /// Counter to keep Explosion image

public:
    Bullet();
    ~Bullet();
    
    inline void SetDirection(Direction_e direction) {
        m_Direction = direction;
    }
    
    int LoadBulletImage(sf::RenderWindow &m_WinMario);
};
