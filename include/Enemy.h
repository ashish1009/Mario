#pragma once

#include "Entity.h"

class Enemy : public Entity {
public:
    enum Type_e {
        DOG = 0,
        DOG_DEAD = 2,
        DUCK = 6,
        DUCK_FLY = 8,
        DUCK_SEMI_DEAD = 10,
        CACTUS = 12,
    };
    
private:
    Type_e m_Type;
    
public:
    int LoadEnemyImage(sf::RenderWindow &window);
    
    Enemy();
    ~Enemy();
};
