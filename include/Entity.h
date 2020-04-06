#pragma once

#include <iostream>
#include "Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Entity Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Structure to define position of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct Position_s {
        int X, Y;
        
        Position_s()
        : X(0.0f), Y(0.0f) {}
        
        Position_s(const int x, const int y)
        : X(x), Y(y) {}
        
        void SetPositionLocal (const int x, const int y) {
            X = x;
            Y = y;
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Size of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Size_e : short {
        SMALL = 0,
        BIG = 1
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Direction of moving Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Direction_e : short {
        LEFT = 0,
        RIGHT = 1
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Abiloty of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Ability_e : short {
        P1_REGULAR = 0, // No Special Ability for Player 1
        P2_REGULAR = 1, // No Special Ability for Player 2
        FIRABLE = 2,    // Entity can Fire
        INVISIBLE = 3,   // Entity can never be Dead
        INCREASING = 4,
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Behaviour of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum State_e : short {
        AIR = 0,        // When Entity is at Free fall
        JUMPING = 1,    // When ENtity is Jumping
        GROUND = 2,     // When Entity is Landed at some place
        DYING = 3,      // When Entity is Dying
        RUNNING = 4,    // When Entity is Running
    };
    
    typedef short Speed;
    static const short DEFAULT_SPEED = 1;
    static const short MAX_SPEED = 2;
    
protected:
    Position_s m_Position;
    Direction_e m_Direction;
    Ability_e m_Ability;
    Size_e m_Size;
    Speed m_Speed;
    State_e m_PrevState;
    State_e m_State;
    
public:
    Entity();
    ~Entity();
    
    virtual bool IsDownCollision (const int frameX) = 0;
    virtual bool IsJumpCollision (const int frameX) = 0;
    virtual bool IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) = 0;
    
    ///////////////////////////////////////////////////////////////
    ///     Get Entity
    ///////////////////////////////////////////////////////////////
    inline const Position_s &GetPosition() const {
        return m_Position;
    }

    inline const Direction_e GetDirection() const {
        return m_Direction;
    }

    inline const Ability_e GetAbility() const {
        return m_Ability;
    }

    inline const Size_e GetSize() const {
        return m_Size;
    }

    inline const Speed GetSpeed() const {
        return m_Speed;
    }

    inline const State_e GetState() const {
        return m_State;
    }
    
    inline const State_e GetPrevState() const {
        return m_PrevState;
    }

    ///////////////////////////////////////////////////////////////
    ///     Set Entity
    ///////////////////////////////////////////////////////////////
    inline void SetPosition(const int X, const int Y) {
        m_Position.X = X;
        m_Position.Y = Y;
    }

    inline void SetDirection(const Direction_e direction) {
        m_Direction = direction;
    }

    inline void SetAbility(const Ability_e ability) {
        m_Ability = ability;
    }

    inline void SetSize(const Size_e size) {
        m_Size = size;
    }

    inline void SetSpeed(const Speed speed) {
        m_Speed = speed;
    }

    inline void SetState(const State_e state) {
        m_State = state;
    }
    
    inline void SetPrevState(const State_e state) { /// store previous state only in case of running and jumping
        m_PrevState = m_State;
    }
};
