#include "Entity.h"
#include "Logger.h"

Entity::Entity()
    : m_Direction(RIGHT), m_Ability(REGULAR), m_Size(SMALL), m_Speed(DEFAULT_SPEED), m_Behaviour(AIR)
{
    LogDebug(BIT_ENTITY, "Entity::Entity(), Entity Constructor called !!\n");
}

Entity::~Entity()
{
    LogDebug(BIT_ENTITY, "Entity::~Entity(), Entity Destructor called !! \n");
}

void Entity::SetPosition(const float X, const float Y)
{
    m_Position.X = X;
    m_Position.Y = Y;
}

void Entity::SetDirection(const Direction_e Direction)
{
    m_Direction = Direction;
}

void Entity::SetAbility(const Abilities_e Ability)
{
    m_Ability = Ability;
}
void Entity::SetSize(const Size_e Size)
{
    m_Size = Size;
}

void Entity::SetSpeed(const Speed speed)
{
    m_Speed = speed;
}

void Entity::SetBehaviour(const Behaviour_e Behaviour)
{
    m_Behaviour = Behaviour;
}
