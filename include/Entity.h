#pragma once

#include <iostream>

const std::string ResourcePath = "/Users/ashish./Documents/XCode/Mario/Mario/Resources/";

class Entity
{
public:
    struct Position_s
    {
        float X;
        float Y;

        Position_s()
        : X(0), Y(0) {}

        Position_s(const float x, const float y)
        : X(x), Y(y) {}
        
        void SetPostion(const float X, const float Y)
        {
            this->X = X;
            this->Y = Y;
        }
    };

    enum Size_e
    {
        SMALL = 0,
        BIG = 1,
    };

    enum Direction_e
    {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3,
    };
    
    enum Abilities_e
    {
        REGULAR = 0,
        REGULAR_PLAYER_2 = 1,
        FIRABLE = 2,
        INVISIBLE = 3,
    };
    
    enum Behaviour_e
    {
        AIR = 0,
        JUMPING = 1,
        GROUND = 2,
        DYING = 3,
    };

    typedef unsigned short Speed;
    static const unsigned short DEFAULT_SPEED = 2;
    static const unsigned short MAX_SPEED = 2;

public:
    Position_s m_Position;
    Direction_e m_Direction;
    Abilities_e m_Ability;
    Size_e m_Size;
    Speed m_Speed;
    Behaviour_e m_Behaviour;

public:
    Entity();
    ~Entity();

    void SetPosition(const float X, const float Y);
    void SetDirection(const Direction_e Direction);
    void SetSize(const Size_e Size);
    void SetAbility(const Abilities_e Ability);
    void SetSpeed(const Speed speed);
    void SetBehaviour(const Behaviour_e Behaviour);
    
    inline const Position_s &GetPosition() const
    {
        return m_Position;
    }

    inline const Direction_e GetDirection() const
    {
        return m_Direction;
    }
    
    inline const Abilities_e GetAbility() const
    {
        return m_Ability;
    }

    inline const Size_e GetSize() const
    {
        return m_Size;
    }

    inline const Speed GetSpeed() const
    {
        return m_Speed;
    }
    
    inline const Behaviour_e GetBehaviour() const
    {
        return m_Behaviour;
    }
};
