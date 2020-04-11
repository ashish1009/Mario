#include "Item.h"
#include "Logger.h"

const int gMaxBrickPath = 42;
const int gBrokeBrickPath1 [gMaxBrickPath][2] = {
    {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2}, {-1, -1}, {-1, -2},
    {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},
    {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1},
};
const int gBrokeBrickPath2 [gMaxBrickPath][2] = {
    {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0}, {-1, -1}, {-1, 0},
    {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},
    {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1}, {-1, 1},
};

Item::Item()
: m_TileVector(BLOCK_SIZE, BLOCK_SIZE), m_IdxY(0), m_IdxX(0), m_pBlock(nullptr), m_ItemBreakPart(0, 0), m_ItemMovePathIdx(0) {
//    LogInfo(BIT_BONUS, "Bonus::Bonus() : Constructor called \n");
}

Item::~Item() {
//    LogInfo(BIT_BONUS, "Bonus::~Bonus() : Destructor called \n");
}

int Item::LoadItemImage(sf::RenderWindow &winMario, short frameX) {
    TileMap::PrintControl_s printControl;

    printControl.tileSet = ResourcePath + BLOCK_IMG_PATH;
    printControl.tile = 0;
    printControl.bInverted = false;
    printControl.tileSize = m_TileVector;
    
    if (Obstacle::NO_ABILITY == m_pBlock->abilities) { /// Some  BRICKES
        if (m_pBlock->bIsPopped) {
            printControl.imgY = Obstacle::BRICK << BLOCK_SIZE_BIT;
            printControl.tile = 1;
            printControl.tileSize = sf::Vector2f(BLOCK_SIZE >> 1, BLOCK_SIZE >> 1);
            printControl.blockBreakPart = GetItemBreakPart();
            
            int brickPart = (GetItemBreakPart().x << 1) + GetItemBreakPart().y; /// to make index 0 as 0,0 : 1 as 0,1 : 2 as 1,0 and 3 as 1,1
            if (m_ItemMovePathIdx++ < gMaxBrickPath) {
                switch (brickPart) {
                    case 0:
                        SetPosition(m_Position.X + gBrokeBrickPath1[m_ItemMovePathIdx][0], m_Position.Y + gBrokeBrickPath1[m_ItemMovePathIdx][1]);
                        break;
                        
                    case 1:
                        SetPosition(m_Position.X - gBrokeBrickPath1[m_ItemMovePathIdx][0], m_Position.Y + gBrokeBrickPath1[m_ItemMovePathIdx][1]);
                        break;
                        
                    case 2:
                        SetPosition(m_Position.X + gBrokeBrickPath2[m_ItemMovePathIdx][0], m_Position.Y + gBrokeBrickPath2[m_ItemMovePathIdx][1]);
                        break;

                    case 3:
                        SetPosition(m_Position.X - gBrokeBrickPath2[m_ItemMovePathIdx][0], m_Position.Y + gBrokeBrickPath2[m_ItemMovePathIdx][1]);
                        break;

                    default:
                        break;
                }
            }
            else {
                (WORLD_VIEW_HEIGHT + BLOCK_SIZE >= GetPosition().Y) ? SetPosition(m_Position.X, m_Position.Y + 2) : SetState(DYING);
            }
        }
    } /// if (Obstacle::NO_ABILITY == m_pBlock->abilities)
    else if (Obstacle::COIN == m_pBlock->abilities) {
        /// Jump coint to 64 Pixels if popped then delete the coin bonus
        (m_IdxY++ < 64) ? SetPosition(m_IdxX - (frameX- m_FramePos), m_Position.Y - 1) : SetState(State_e::DYING);
        printControl.imgY = ((m_IdxY % 2) + Obstacle::COIN) << BLOCK_SIZE_BIT;
    }
    else if (Obstacle::MUSHROOM == m_pBlock->abilities) {
        Player *pPlayer = Player::GetInstance();  /// Doesnt create new instance if already created it return th epointer else null
        if (!pPlayer) {
            LogError(BIT_BONUS, "Bonus::LoadBonusImage(), Player Object doesnt exist \n");
            return EXIT_FAILURE;
        }
        if (SMALL == pPlayer->GetSize()) {
            printControl.imgY = Obstacle::MUSHROOM << BLOCK_SIZE_BIT;
            if (AIR == m_State) {
                (IsDownCollision(frameX)) ? SetState(GROUND) : SetPosition(m_Position.X, m_Position.Y + 1);
            }
            else {
                int xPixel = ((RIGHT == m_Direction) ? m_Position.X + BLOCK_SIZE : m_Position.X);
                (IsSideCollision(frameX, BLOCK_SIZE, xPixel)) ? SetDirection(LEFT) :  SetPosition(m_Position.X + m_Direction, m_Position.Y);
                SetState(AIR);
            }
        }
        else { /// if Player is Big then Mushroom is replaced with Fire Flower
            printControl.imgY = Obstacle::FIRE_LEAFE << BLOCK_SIZE_BIT;
            SetPosition(m_IdxX - (frameX - m_FramePos), m_Position.Y);
        } ///if (SMALL == pPlayer->GetSize())
        
        if (IsPlayerCollision()) {
            SetState(DYING);
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
bool Item::IsPlayerCollision() {
    bool bResult = false;
    Player *pPlayer = Player::GetInstance();
    
    if (pPlayer->GetPosition().X + Player::PLAYER_WIDTH == m_Position.X) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (pPlayer->GetPosition().Y - i == m_Position.Y - BLOCK_SIZE + 1) {  /// Add 1 for first pixel from Up
                bResult = true;
                break;
            }
        }
    }
    else if (pPlayer->GetPosition().X == m_Position.X + BLOCK_SIZE - 1) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            if (pPlayer->GetPosition().Y - i == m_Position.Y - BLOCK_SIZE + 1) {
                bResult = true;
                break;
            }
        }
    }
    else if (pPlayer->GetPosition().Y - pPlayer->GetPlayerHeight() == m_Position.Y) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            const int playerPixelX = pPlayer->GetPosition().X + i;
            if ((playerPixelX == m_Position.X) || (playerPixelX == m_Position.X + BLOCK_SIZE - 1)) {
                bResult = true;
                break;
            }
        }
    }
    else if (pPlayer->GetPosition().Y == m_Position.Y - BLOCK_SIZE) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            const int playerPixelX = pPlayer->GetPosition().X + i;
            if ((playerPixelX == m_Position.X) || (playerPixelX == m_Position.X + BLOCK_SIZE - 1)) {
                bResult = true;
                break;
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

bool Item::IsDownCollision (const int frameX) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsLeftDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedL + m_Position.X + frameX);
    bool bIsRightDownCollision = pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedR + m_Position.X + frameX);
                            
    return (bIsLeftDownCollision || bIsRightDownCollision);
}

bool Item::IsJumpCollision (const int frameX) {
    return false;
}

bool Item::IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) {
    Obstacle *pObstacle = Obstacle::GetInstance();
    bool bIsSideUpCollision = pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX);
    bool bIsSideDownCollision = pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX);
    
    return (bIsSideUpCollision || bIsSideDownCollision);
}
