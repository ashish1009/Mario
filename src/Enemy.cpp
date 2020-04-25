#include "Enemy.h"
#include "Logger.h"
#include "Player.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Constructor
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy()
: m_Type(DOG) {
//    LogInfo (BIT_ENEMY, "Enemy::Enemy(), Constructor called !!!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Destructor
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::~Enemy() {
//    LogInfo (BIT_ENEMY, "Enemy::~Enemy(), Destructor called !!!\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Load Enemy
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Enemy::LoadEnemyImage(sf::RenderWindow &window) {
    TileMap::PrintControl_s printControl;
    printControl.imgXIdx = m_Type + (static_cast<short>(m_Position.X) % 2);
    printControl.imgYIdx = BLOCK_SIZE;
    printControl.imgType = TileMap::ENEMY;
    printControl.tileSize = m_TileVector;
    printControl.xTileOffset = BLOCK_SIZE_BIT;
    
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
    
    printControl.position = m_Position;
    
    Player *pPlayer = Player::GetInstance();
    if (IsEntityCollision(pPlayer)) {
        if (INVISIBLE_STATE != pPlayer->GetState()) {
            Player::GetInstance()->SetState(DYING);
        }
    }

    m_Map.Load(printControl);
    window.draw(m_Map);
    
    return EXIT_SUCCESS;
}
