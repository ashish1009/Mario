#pragma once
#include "TileMap.h"
#include "Obstacle.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Player Image Index
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace PlayerImgIdx {
static const short RUN_IDX_ARR_SIZE = 3;
static const short STAND = 0;
static const short DYING = 6;
static const short RUN[RUN_IDX_ARR_SIZE] = {1, 2, 3};
static const short JUMP = 5;
static const short MID_INC = 15;
}

const short gPixelToBeColloidedL = 4;
const short gPixelToBeColloidedM = BLOCK_SIZE >> 1;
const short gPixelToBeColloidedR = BLOCK_SIZE - gPixelToBeColloidedL;

const short gPixelToColloidD = 4;
const float gMoveFactor = 0.5;
const float gItemMoveFactor = 0.25;

const float gJumpFallFactor = 0.25f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Entity Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity {
public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Direction of moving Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Direction_e : short {
        LEFT = -1,
        RIGHT = 1
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Size of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Size_e : short {
        SMALL = 0,
        BIG = 1
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define Abilty of Entity (Multiplied by 48 to jump appropriate image)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum Ability_e : short {
        P1_REGULAR = 0,     // No Special Ability for Player 1
        P2_REGULAR = 48,     // No Special Ability for Player 2
        FIRABLE = 96,        // Entity can Fire
        INVISIBLE = 144,      // Entity can never be Dead
        CHANGING_STATE = 192,
    };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum to define State of Entity
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum State_e : short {
        AIR = 0,
        GROUND = 1,
        RUNNING = 2,
        JUMPING = 3,
        DYING = 4,
        SHOOTING = 5,
    };

    static const short DEFAULT_SPEED = 0;
    
protected:
    Position_s m_Position;
    Direction_e m_Direction;
    Ability_e m_Ability;
    Size_e m_Size;
    State_e m_State;
    State_e m_PrevState;
    
    short m_Speed;
    short m_Height;
    
    sf::Vector2<float> m_TileVector;
    
    TileMap m_Map;
    Obstacle *m_pObstacle;
    
public:
    Entity();
    ~Entity();
    
    bool IsDownCollision();
    bool IsJumpCollision();
    bool IsSideCollision();
    
    void IncSize();
    
    inline void SetPosition (const float X, const float Y) {
        m_Position.X = X;
        m_Position.Y = Y;
    }
    
    inline void SetDirection (Direction_e direction) {
        m_Direction = direction;
    }
    
    inline void ToggleDirection() {
        m_Direction = ((LEFT == m_Direction) ? RIGHT : LEFT);
    }
    
    inline void SetAbility (Ability_e ability) {
        m_Ability = ability;
    }
    
    inline void SetSize (Size_e size) {
        m_Size = size;
    }
        
    inline void SetState (State_e state) {
        m_State = state;
    }
    
    inline void SetPrevState (State_e state) {
        m_PrevState = state;
    }
    
    inline void SetSpeed (short speed) {
        m_Speed = speed;
    }

    inline void SetHeight (short height) {
        m_Height = height;
    }
    
    inline Position_s GetPosition () const {
        return m_Position;
    }
    
    inline Direction_e GetDirection () const {
        return m_Direction;
    }
    
    inline Ability_e GetAbility () const {
        return m_Ability;
    }
    
    inline Size_e GetSize () const {
        return m_Size;
    }
    
    inline State_e GetState () const {
        return m_State;
    }

    inline State_e GetPrevState () const {
        return m_PrevState;
    }

    inline short GetSpeed () const {
        return m_Speed;
    }
    
    inline short GetHeight() const {
        return m_Height;
    }
};
