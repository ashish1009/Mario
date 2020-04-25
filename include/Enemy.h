#pragma once

#include "Entity.h"
#include "SoundBuffer.h"

enum Type_e {
    DOG = 0,
    DOG_DEAD = 2,
    DUCK = 6,
    DUCK_FLY = 8,
    DUCK_SEMI_DEAD = 10,
    CACTUS = 12,
};

struct EnemyPosType_s {
    Type_e type;
    short xPos;
};

class Enemy : public Entity, SoundBuffer {
private:
    bool m_bPlayerCollision;
    bool m_bIsJumpDying;
    bool m_bIsBulletDying;
    short m_DyingTime;
    short m_UpHeight;
    Type_e m_Type;
    
public:
    int LoadEnemyImage(sf::RenderWindow &window);
    
    inline void SetBulletDying() {
        m_bIsBulletDying = true;
    }
    inline void SetType(Type_e type) {
        m_Type = type;
    }
    
    Enemy();
    ~Enemy();
};
