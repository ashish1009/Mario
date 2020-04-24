#include "Entity.h"
#include "Logger.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::Entity()
: m_Direction(RIGHT), m_Ability(P1_REGULAR), m_Size(SMALL), m_State(AIR), m_Speed(DEFAULT_SPEED), m_Height(BLOCK_SIZE), m_TileVector(BLOCK_SIZE, BLOCK_SIZE), m_pObstacle(nullptr) {
    m_pObstacle = Obstacle::GetInstance();
//    LogInfo(BIT_ENTITY, "Entity::Entity(), Constructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Entity::~Entity() {
    m_pObstacle = nullptr;
//    LogInfo(BIT_ENTITY, "Entity::~Entity(), Destructor called !!! \n");
}

void Entity::IncSize() {
    m_Size = BIG;
    m_Height = (BLOCK_SIZE << 1);
    m_TileVector = {BLOCK_SIZE, (BLOCK_SIZE << 1)};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the landing collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::IsDownCollision () {
    bool bIsLeftDownCollision = m_pObstacle->GetObstacle(m_Position.Y, m_Position.X + gPixelToBeColloidedL - 1);
    bool bIsRightDownCollision = m_pObstacle->GetObstacle(m_Position.Y, m_Position.X + gPixelToBeColloidedR);

    if (bIsLeftDownCollision || bIsRightDownCollision) {
        return true;
    }
    
    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetObstacle(m_Position.Y, m_Position.X + gPixelToBeColloidedL - 2)) && (!bIsRightDownCollision)) {
        m_Position.X += (gPixelToBeColloidedL - 1);
        SetState(AIR);
        return false;
    }

    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetObstacle(m_Position.Y, m_Position.X + gPixelToBeColloidedR + 1)) && (!bIsLeftDownCollision)) {
        m_Position.X -= (gPixelToBeColloidedL - 1);
        SetState(AIR);
        return false;
    }
    else {
        return  false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the landing collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::IsJumpCollision () {
    bool bIsDownCollision1 = m_pObstacle->GetObstacle(m_Position.Y - m_Height, m_Position.X + gPixelToBeColloidedM - 1);
    bool bIsDownCollision2 = m_pObstacle->GetObstacle(m_Position.Y - m_Height, m_Position.X + gPixelToBeColloidedM);

    if (bIsDownCollision1 || bIsDownCollision2) {
        m_pObstacle->SetPoppedBlock(static_cast<short>(m_Position.Y - m_Height) >> BLOCK_SIZE_BIT, static_cast<short>(m_Position.X + gPixelToBeColloidedM) >> BLOCK_SIZE_BIT, (BIG == m_Size));
        return true;
    }
    
    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetObstacle(m_Position.Y - m_Height, m_Position.X + gPixelToBeColloidedL - 2)) && (!bIsDownCollision1)) {
        m_Position.X += (gPixelToBeColloidedM - 1);
        return false;
    }

    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetObstacle(m_Position.Y - m_Height , m_Position.X + gPixelToBeColloidedR + 1)) && (!bIsDownCollision2)) {
        m_Position.X -= (gPixelToBeColloidedL - 1);
        return false;
    }
    else {
        return  false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the Side collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entity::IsSideCollision () {
    const short midPixel = m_Height >> 1;
    const short pixelToColloidU = m_Height - gPixelToColloidD;
    const short xPixelOfPlayer = (RIGHT == m_Direction) ? (m_Position.X + BLOCK_SIZE) : (m_Position.X - 1);

    bool bIsSideUpCollision = m_pObstacle->GetObstacle(m_Position.Y - pixelToColloidU, xPixelOfPlayer);
    bool bIsSideDownCollision = m_pObstacle->GetObstacle(m_Position.Y - gPixelToColloidD, xPixelOfPlayer);
    bool bIsSideMidCollision = m_pObstacle->GetObstacle(m_Position.Y - midPixel, xPixelOfPlayer);

    if (bIsSideUpCollision || bIsSideDownCollision || bIsSideMidCollision) {
        return true;
    }

    /// Shift the plyer Up if few pixels are Down for landing and those pixels are not enough for for side collision
    else if (m_pObstacle->GetObstacle(m_Position.Y - gPixelToColloidD + 3, xPixelOfPlayer) && (!bIsSideDownCollision)) {
        m_Position.Y -= gPixelToColloidD;
        return false;
    }

    else {
        return false;
    }
}
