#include "Bonus.h"
#include "Logger.h"

Bonus *Bonus::m_pInstance = nullptr;

Bonus::Bonus()
: m_TileVector(BLOCK_SIZE, BLOCK_SIZE), m_IdxY(0), m_IdxX(0), m_pBlock(nullptr) {
//    LogInfo(BIT_BONUS, "Bonus::Bonus() : Constructor called \n");
}

Bonus::~Bonus() {
//    LogInfo(BIT_BONUS, "Bonus::~Bonus() : Destructor called \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Create new Instance for Bonus. If alredy created then return the older Pointer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bonus *Bonus::CreateInstance() {
    if (nullptr == m_pInstance) {
        LogInfo(BIT_BONUS, "Bonus::GetInstance(), Creating Bonus Instance() \n");
        m_pInstance = new Bonus();
    }
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : If alredy created then return the Pointer else null
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bonus *Bonus::GetInstance() {
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : delete the created Instance
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Bonus *Bonus::ReleaseInstance() {
    if (nullptr != m_pInstance) {
        LogInfo(BIT_BONUS, "Bonus::ReleaseInstance(), Deleting Bonus Instance() \n");
        delete m_pInstance;
        m_pInstance = nullptr;
    }
    return m_pInstance;
}

int Bonus::LoadBlockImage(sf::RenderWindow &winMario, short frameX, Obstacle::ObstacleBlock_s *block) {
    if (Obstacle::NO_ABILITY == block->abilities) {
        return EXIT_SUCCESS;
    }
    TileMap::PrintControl_s printControl;

    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.tile = 0;
    printControl.bInverted = false;
    printControl.tileSize = m_TileVector;

    if (Obstacle::COIN == block->abilities) {
        if (m_IdxY++ < 64) {
            SetPosition(m_IdxX - (frameX- m_FramePos), m_Position.Y - 1);
        }
        else {
            SetState(State_e::DYING);
        }
        printControl.imgY = ((m_IdxY % 2) + Obstacle::COIN_FRONT) << BLOCK_SIZE_BIT;
    }
    else if (Obstacle::MUSHROOM == block->abilities) {
        Player *pPlayer = Player::GetInstance();  /// Doesnt create new instance if already created it return th epointer else null
        if (pPlayer) {
            if (SMALL == pPlayer->GetSize()) {
                printControl.imgY = Obstacle::MUSHROOM << BLOCK_SIZE_BIT;
                if (AIR == m_State) {
                    (IsDownCollision(frameX)) ? SetState(GROUND) : SetPosition(m_Position.X, m_Position.Y + 1);
                } /// if (AIR == m_State)
                else {
                    int xPixel = ((RIGHT == m_Direction) ? m_Position.X + BLOCK_SIZE : m_Position.X);
                    (IsSideCollision(frameX, BLOCK_SIZE, xPixel)) ? SetDirection(LEFT) :  SetPosition(m_Position.X + m_Direction, m_Position.Y);
                    SetState(AIR);
                }
            }
            else { /// if Player is Big then Mushroom is replaced with Fire Flower
                printControl.imgY = Obstacle::FIRE_BONUS << BLOCK_SIZE_BIT;
                SetPosition(m_IdxX - (frameX- m_FramePos), m_Position.Y);
            } ///if (SMALL == pPlayer->GetSize())
            
            if (IsPlayerCollision()) {
                m_State = DYING;
            }
        } /// if (pPlayer)
        else {
            LogError(BIT_BONUS, "Bonus::LoadBlockImage() : No Player Exist \n");
            return EXIT_FAILURE;
        }
    } /// else if (Obstacle::MUSHROOM == block->abilities)
    
    printControl.position = m_Position;
    if (!m_Map.Load(printControl)) {
        LogError (BIT_BLOCK, "Block::LoadBlockImage() : Can Not Load Block Image \n");
        return EXIT_FAILURE;
    }
    
    winMario.draw(m_Map);
    return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Detect the collision with player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Bonus::IsPlayerCollision() {
    bool bResult = false;
    Player *pPlayer = Player::GetInstance();
    
    if (pPlayer->GetPosition().X + Player::PLAYER_WIDTH == m_Position.X) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (pPlayer->GetPosition().Y - i == m_Position.Y - BLOCK_SIZE + 1) {  /// Add 1 for first pixel from Up
                bResult = true;
            }
        }
    }
    else if (pPlayer->GetPosition().X == m_Position.X + BLOCK_SIZE - 1) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (pPlayer->GetPosition().Y - i == m_Position.Y - BLOCK_SIZE + 1) {
                bResult = true;
            }
        }
    }
    else if (pPlayer->GetPosition().Y - pPlayer->GetPlayerHeight() == m_Position.Y) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            const int playerPixelX = pPlayer->GetPosition().X + i;
            if ((playerPixelX == m_Position.X) || (playerPixelX == m_Position.X + BLOCK_SIZE - 1)) {
                bResult = true;
            }
        }
    }
    else if (pPlayer->GetPosition().Y == m_Position.Y - BLOCK_SIZE) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            const int playerPixelX = pPlayer->GetPosition().X + i;
            if ((playerPixelX == m_Position.X) || (playerPixelX == m_Position.X + BLOCK_SIZE - 1)) {
                bResult = true;
            }
        }
    }
    if (bResult) {
        if (m_pBlock->abilities == Obstacle::MUSHROOM) {
            (SMALL == pPlayer->GetSize()) ? pPlayer->IncreaseSize() : pPlayer->SetAbility(FIRABLE);
        }
    }
    return bResult;
}

bool Bonus::IsDownCollision (const int frameX) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsLeftDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedL + m_Position.X + frameX);
    bool bIsRightDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedR + m_Position.X + frameX);
                            
    if (bIsLeftDownCollision || bIsRightDownCollision) {
        return true;
    }
    else {
        return  false;
    }
}

bool Bonus::IsJumpCollision (const int frameX) {
    return false;
}

bool Bonus::IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsSideUpCollision = pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX);
    bool bIsSideDownCollision = pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX);
    
    if (bIsSideUpCollision || bIsSideDownCollision) {
        return true;
    }
    else {
        return false;
    }
}
