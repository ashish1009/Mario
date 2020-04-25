#include "Enemy.h"
#include "Logger.h"
#include "Player.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Constructor
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy()
: m_bPlayerCollision(false), m_bIsJumpDying(false), m_bIsBulletDying(false), m_DyingTime(0), m_UpHeight(0), m_Type(DOG) {
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
    Player *pPlayer = Player::GetInstance();
    
    auto it = pPlayer->m_lBullet.begin();
    for (; it != pPlayer->m_lBullet.end(); it++) {
        if (it->IsEntityCollision(this)) {
            this->m_bIsBulletDying = true;
        }
    }
        
    if (AIR == m_State) {
        if (IsDownCollision()) {
            SetState(GROUND);
        }
        else {
            SetPosition(m_Position.X, m_Position.Y + gJumpFallFactor);
        }
    }
    else {
        if (m_bIsBulletDying) {
            if (m_UpHeight < 64) {
                m_Position.Y -= 0.5;
                m_UpHeight += 0.5;
            }
            else {
                m_Position.Y += 0.5;
            }
            if (m_Position.Y < 0) {
                this->m_State = DYING;
            }
        }
        else if (!m_bIsJumpDying) {
            int xPixel = ((RIGHT == m_Direction) ? m_Position.X + BLOCK_SIZE : m_Position.X);
            if (IsSideCollision()) {
                ToggleDirection();
            }
            else {
                (RIGHT == m_Direction) ? m_Position.X += 0.1 : m_Position.X -= 0.1;
                SetState(AIR);
            }
        }
    }
    
    if ((!m_bIsJumpDying) && (!m_bIsBulletDying)) {
        if (!m_bPlayerCollision) {
            if (IsEntityCollision(pPlayer)) {
                if (pPlayer->GetPosition().Y <= this->m_Position.Y - BLOCK_SIZE) {
                    m_bIsJumpDying = true;
                }
                else
                {
                    m_Sound.setBuffer(m_DeathSound);
                    m_Sound.play();

                    pPlayer->SetState(DYING);
                    m_bPlayerCollision = true;
                }
            }
        }
        else {
            if (!pPlayer->GetIsVissble()) {
                m_bPlayerCollision = false;
            }
        }
    }
    else {
        if (m_DyingTime++ > 1000) {
            m_State = DYING;
        }
    }
    
    if (m_bIsJumpDying){
        printControl.imgXIdx = m_Type + 5;
    }
    else {
        printControl.imgXIdx = m_Type + (static_cast<short>(m_Position.X) % 2);
    }
    
    if (DUCK == m_Type) {
        m_TileVector = {BLOCK_SIZE, (BLOCK_SIZE << 1)};
        printControl.imgYIdx = 0;
    }
    else {
        printControl.imgYIdx = BLOCK_SIZE;
    }
    
    printControl.position = m_Position;
    printControl.imgType = TileMap::ENEMY;
    printControl.tileSize = m_TileVector;
    printControl.xTileOffset = BLOCK_SIZE_BIT;

    m_Map.Load(printControl);
    window.draw(m_Map);
    
    return EXIT_SUCCESS;
}
