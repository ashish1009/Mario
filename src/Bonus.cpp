#include "Bonus.h"
#include "Logger.h"

Bonus *Bonus::m_pInstance = nullptr;

Bonus::Bonus()
: m_IdxY(0), m_pBlock(nullptr) {
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
//        LogInfo(BIT_BONUS, "Bonus::GetInstance(), Creating Bonus Instance() \n");
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
//        LogInfo(BIT_BONUS, "Bonus::ReleaseInstance(), Deleting Bonus Instance() \n");
        delete m_pInstance;
        m_pInstance = nullptr;
    }
    return m_pInstance;
}

int Bonus::LoadBlockImage(sf::RenderWindow &winMario, short frameX, Obstacle::ObstacleBlock_s *block) { /// TODO :  if bonus is not used  then rmv it frm here & from bonus class m_Block
    static int ii = 0;
    TileMap::PrintControl_s printControl;

    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.tile = 0;
    printControl.bInverted = false;
    printControl.tileSize = sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE);

    if (Obstacle::COIN == block->abilities) {
        if (m_IdxY++ < 64) {
            SetPosition(m_IdxX - (frameX- m_FramePos), m_Position.Y - 1);
        }
        else {
            SetState(State_e::DYING);
        }
        
        Obstacle::Behaviour_e objBehav = Obstacle::Behaviour_e((m_IdxY % 2) + Obstacle::COIN_FRONT);
        
        printControl.imgY = objBehav << BLOCK_SIZE_BIT;
    } /// if (Obstacle::COIN == block->abilities)
    else if (Obstacle::MUSHROOM == block->abilities) {
        Player *pPlayer = Player::GetInstance();  /// Doesnt create new instance if already created it return th epointer else null
        printControl.imgY = block->abilities << BLOCK_SIZE_BIT;

        if (pPlayer) {
            if (SMALL == pPlayer->GetSize()) {
                if (AIR == m_State) {
                    if (IsDownCollision(frameX)) {
                        SetState(GROUND);
                    }
                    else {
                        SetPosition(m_Position.X, m_Position.Y + 1);
                    }
                } /// if (AIR == m_State)
                else {
                    int xPixel = m_Position.X;
                    if (RIGHT == m_Direction) {
                        SetPosition(m_Position.X + 1, m_Position.Y);
                        xPixel += BLOCK_SIZE;
                    }
                    else {
                        SetPosition(m_Position.X - 1, m_Position.Y);
                    }
                    
                    if (IsSideCollision(frameX, BLOCK_SIZE, xPixel)) {
                        SetDirection(LEFT);
                    }
                    if (DYING != m_State) {
                        SetState(AIR);
                    }
                }
            }
            else { /// if Player is Big then Mushroom is replaced with Fire
                printControl.imgY = Obstacle::FIRE_BONUS << BLOCK_SIZE_BIT;
                SetPosition(m_IdxX - (frameX- m_FramePos), m_Position.Y);
            } ///if (SMALL == pPlayer->GetSize())
            if (IsBonusCollion(pPlayer)) {
                m_State = DYING;
                printControl.tile = 3;
            }
        } /// if (pPlayer)
        else {
            LogError(BIT_BONUS, "Bonus::LoadBlockImage() : No Player Exist \n");
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

bool Bonus::IsBonusCollion(Player *pPlayer) {
    bool bResult = false;
    
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
    if (bResult) {
        if (m_pBlock->abilities == Obstacle::MUSHROOM) {
            if (SMALL == pPlayer->GetSize()) {
                pPlayer->IncreaseSize();
            }
            else {
                pPlayer->SetAbility(FIRABLE);
            }
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
