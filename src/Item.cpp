#include "Item.h"
#include "Logger.h"
#include "Player.h"

#define fac1 1
#define fac2 2

const short gMaxBrickPath = 42;
const float gBrokeBrickPath1 [gMaxBrickPath][2] = {
    {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2}, {-fac1, -fac1}, {-fac1, -fac2},
    {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1},
    {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1},
};
const float gBrokeBrickPath2 [gMaxBrickPath][2] = {
    {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0}, {-fac1, -fac1}, {-fac1, 0},
    {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1}, {-fac1, -fac1},
    {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1}, {-fac1, fac1},
};

Item::Item()
:m_BrickPartNum(0), m_BrickPartMovePathIdx(0), m_CoinJumpIdx(0) {
//    LogInfo(BIT_BONUS, "Bonus::Bonus() : Constructor called \n");
}

Item::~Item() {
//    LogInfo(BIT_BONUS, "Bonus::~Bonus() : Destructor called \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Draw jumping coin
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Item::DrawCoinJump (short &imgY) {
    if (m_CoinJumpIdx < 64) {
        m_Position.Y -= gJumpFallFactor;
        m_CoinJumpIdx += gJumpFallFactor;
    }
    else {
        SetState(State_e::DYING);
    }
    imgY = ((static_cast<short>(m_CoinJumpIdx) % 2) + COIN) << BLOCK_SIZE_BIT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Draw Mushroom or fire leafe
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Item::DrawBonus(short &imgY) {
    Player *pPlayer = Player::GetInstance();
    
    if (SMALL == pPlayer->GetSize()) {
        imgY = MUSHROOM << BLOCK_SIZE_BIT;
        if (AIR == m_State) {
            if (IsDownCollision()) {
                SetState(GROUND);
            }
            else {
                SetPosition(m_Position.X, m_Position.Y + gJumpFallFactor);
            }
        }
        else {
            int xPixel = ((RIGHT == m_Direction) ? m_Position.X + BLOCK_SIZE : m_Position.X);
            if (IsSideCollision()) {
                ToggleDirection();
            }
            else {
                (RIGHT == m_Direction) ? m_Position.X += gItemMoveFactor : m_Position.X -= gItemMoveFactor;
                SetState(AIR);
            }
        }
    }
    else { /// if Player is Big then Mushroom is replaced with Fire Flower
        imgY = FIRE_LEAF << BLOCK_SIZE_BIT;
    }
    
    if (IsEntityCollision(Player::GetInstance())) {
        if (m_pBlock->m_Abilty == MUSHROOM_BONUS) {
            (SMALL == pPlayer->GetSize()) ? pPlayer->ToggleSize() : pPlayer->SetAbility(FIRABLE);
        }
        this->m_State = DYING;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Draw broken brick piece
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Item::DrawBrokenBrick(TileMap::PrintControl_s &printControl) {
    printControl.tileSize = sf::Vector2f(BLOCK_SIZE_SMALL, BLOCK_SIZE_SMALL);
    printControl.xTileOffset = BLOCK_SIZE_SMALL_BIT;
    printControl.imgYIdx = ((BRICKS << 1) + ((m_BrickPartNum >> 1) ? 1 : 0)) << BLOCK_SIZE_SMALL_BIT;
    printControl.imgXIdx = ((m_BrickPartNum & 1) ? 5 : 4);
    
    if (m_BrickPartMovePathIdx++ < gMaxBrickPath) {
        switch (m_BrickPartNum) {
            case 0:
                SetPosition(m_Position.X + gBrokeBrickPath1[m_BrickPartMovePathIdx][0], m_Position.Y + gBrokeBrickPath1[m_BrickPartMovePathIdx][1]);
                break;

            case 1:
                SetPosition(m_Position.X - gBrokeBrickPath1[m_BrickPartMovePathIdx][0], m_Position.Y + gBrokeBrickPath1[m_BrickPartMovePathIdx][1]);
                break;

            case 2:
                SetPosition(m_Position.X + gBrokeBrickPath2[m_BrickPartMovePathIdx][0], m_Position.Y + gBrokeBrickPath2[m_BrickPartMovePathIdx][1]);
                break;

            case 3:
                SetPosition(m_Position.X - gBrokeBrickPath2[m_BrickPartMovePathIdx][0], m_Position.Y + gBrokeBrickPath2[m_BrickPartMovePathIdx][1]);
                break;

            default:
                break;
        }
    }
    else {
        (VIEW_HEIGHT + BLOCK_SIZE >= GetPosition().Y) ? SetPosition(m_Position.X, m_Position.Y + 2) : SetState(DYING);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Draw Items once nlock is popped
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Item::LoadItemImage(sf::RenderWindow &window) {
    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = 0;
    printControl.imgType = TileMap::ITEM;
    printControl.tileSize = m_TileVector;
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    
    if (COIN_BONUS == m_pBlock->m_Abilty) {
        DrawCoinJump(printControl.imgYIdx);
    }
    else if (MUSHROOM_BONUS == m_pBlock->m_Abilty) {
        DrawBonus(printControl.imgYIdx);
    }
    else if (BREAKABLE == m_pBlock->m_Abilty) {
        DrawBrokenBrick(printControl);
    }
    else {
        return EXIT_FAILURE;
    }
    
    printControl.position = m_Position;

    m_Map.Load(printControl);
    window.draw(m_Map);

    return EXIT_SUCCESS;
}
