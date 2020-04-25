#include "Player.h"
#include "Logger.h"

Player *Player::m_Instance = nullptr;

const short PLAYER_START_X = 40;
const short PLAYER_START_Y = 199;
const short FRAME_MOVE_LIMIT = 140;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::Player()
: m_bDying(false), m_bInvisible(false), m_bShotFire(false), m_bLifeReduced(false), m_Life(DEFAULT_PLAYER_LIFE), m_ImgIdx(PlayerImgIdx::STAND), m_PlayerMoveIdx(0), m_JumpFactor(0.0f) {
    SetPosition(PLAYER_START_X, PLAYER_START_Y);
    SetHeight(PLAYER_HEIGHT_SMALL);
    
//    ToggleSize();
//    SetAbility(FIRABLE);
    LogInfo(BIT_PLAYER, "Player::Player(), Constructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Destructor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::~Player() {
    LogInfo(BIT_PLAYER, "Player::~Player(), Destructor called !!! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Create a new instance if not created already and return
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player *Player::GetInstance() {
    if (!m_Instance) {
        m_Instance = new Player;
        LogInfo(BIT_PLAYER, "Player::GetInstance(), Creating Player Instance \n");
    }
    return m_Instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Delete instance if not deleted
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player *Player::ReleaseInstance() {
    if (m_Instance) {
        delete m_Instance;
        m_Instance = nullptr;
        LogInfo(BIT_PLAYER, "Player::ReleaseInstance(), Deleting Player Instance \n");
    }
    return m_Instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Load the PlayerImage
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::LoadImage(sf::RenderWindow &window) {
    CheckState(window);

    if (!m_bDying) {
        TileMap::PrintControl_s printControl;
        printControl.imgXIdx = m_ImgIdx;
        printControl.imgYIdx = (SMALL == m_Size) ? m_Ability + PLAYER_HEIGHT_BIG : m_Ability;
        printControl.position = m_Position;
        printControl.xTileOffset = BLOCK_SIZE_BIT;
        printControl.tileSize = m_TileVector;
        printControl.bIsInverted = (RIGHT == m_Direction) ? false : true;
        printControl.imgType = TileMap::PLAYER;
        
        if (m_bInvisible) {
            static unsigned short invisibleTime = 0;
            printControl.imgXIdx = (invisibleTime++ % 2) * 14;
         
            if (invisibleTime > 1000) {
                invisibleTime = 0;
                m_bInvisible = false;
            }
        }
        
        m_Map.Load(printControl);
        window.draw(m_Map);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Dyig player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::DrawDyingPlayer(sf::RenderWindow &window) {
    if (!m_bInvisible) {
        if (m_JumpFactor < 64) {
            m_Position.Y -= 0.2;
            m_JumpFactor += 0.2;
        }
        else {
            m_Position.Y += 0.2;
        }
                    
        TileMap::PrintControl_s printControl;
        printControl.imgXIdx = 6;
        printControl.imgYIdx = PLAYER_HEIGHT_BIG;
        printControl.xTileOffset = BLOCK_SIZE_BIT;
        printControl.tileSize = m_TileVector;
        printControl.imgType = TileMap::PLAYER;
        printControl.position = m_Position;
      
        if (m_bInvisible) {
            static unsigned short invisibleTime = 0;
            printControl.imgXIdx = (invisibleTime++ % 2) * 14;
         
            if (invisibleTime > 1000) {
                invisibleTime = 0;
                m_bInvisible = false;
            }
        }
        
        m_Map.Load(printControl);
        window.draw(m_Map);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Check the current state of Player and perform Action
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::CheckState(sf::RenderWindow &window) {\
    DrawBullets(window);
    
    if (DYING == m_State) {
        if (BIG == m_Size) {
            m_State = INVISIBLE_STATE;
            m_bInvisible = true;
            ToggleSize();
        }
        else {
            m_bDying = true;
            DrawDyingPlayer(window);
        }
    }
    else if ((JUMPING == m_State) || ((JUMPING == m_PrevState) && (SHOOTING == m_State))) {
        SetImgIdx(PlayerImgIdx::JUMP);
        Jump();
    }
    else {
        if ((GROUND == m_State) && (JUMPING == m_PrevState)) {
            SetImgIdx(PlayerImgIdx::STAND);
        }
        else {
            if (SHOOTING == m_State) {
                m_ImgIdx = PlayerImgIdx::MID_INC + 1;
                if (m_bShotFire) {
                    SetState(AIR);
                }
            }
        }
        
        if (!IsDownCollision()) {
            m_Position.Y += 0.5;
            SetState(AIR);
        }
        else {
            SetState(GROUND);
            m_Position.X = static_cast<int>(m_Position.X);
        }
    }
    
    if (m_Position.Y >= 250) {
        m_Life--;
        m_bLifeReduced = true;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Jump Player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Jump() {
    if (!m_bDying) {
        if (MAX_JUMP_HEIGHT < m_JumpFactor) {
            SetState(AIR);                                      /// On reaching heighest lvel change state to Air so tha it may free fall
            SetPrevState(JUMPING);
            m_JumpFactor = 0.0f;
        }
        else {
            if (!IsJumpCollision()) {
                m_JumpFactor += gJumpFallFactor;
                m_Position.Y -= gJumpFallFactor;
            }
            else {
                SetState(AIR);                                  /// On collision change state to Air so tha it may free fall
                SetPrevState(JUMPING);
                m_JumpFactor = 0.0f;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Move The Player
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Move(Entity::Direction_e directiom, float &frameX) {
    if (!m_bDying) {
        SetDirection(directiom);
        IncSpeed();
        
        if (GROUND == m_State) {
            SetState(RUNNING);
        }

        for (int i = 0; i < m_Speed; i++) {
            if ((JUMPING == m_State) || ((JUMPING == m_PrevState) && (SHOOTING == m_State))) {
                Jump();
            }
            else if (RUNNING == m_State) {
                unsigned short i = (m_PlayerMoveIdx++) >> 1;
                SetImgIdx(PlayerImgIdx::RUN[i %= PlayerImgIdx::RUN_IDX_ARR_SIZE]);
            }
            
            if (!IsSideCollision()) {
                if (RIGHT == m_Direction) {
                    m_Position.X += ((i == 0) ? 1 : gMoveFactor);
                    if ((m_Position.X - frameX) > FRAME_MOVE_LIMIT) {
                        frameX += ((i == 0) ? 1 : gMoveFactor);
                    }
                }
                else {
                    if ((m_Position.X - frameX) > 0) {
                        m_Position.X -= ((i == 0) ? 1 : gMoveFactor);
                    }
                }
            }
            else {
                break;
            }
        }
        SetPrevState(RUNNING);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Fire the bullet
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Fire() {
    if (FIRABLE == m_Ability) {
        m_Sound.setBuffer(m_FireSound);
        m_Sound.play();

        Bullet bullet;
        
        bullet.SetDirection(m_Direction);
        if (RIGHT == m_Direction) {
            bullet.SetPosition(m_Position.X + PLAYER_WIDTH, m_Position.Y - (m_Height >> 1));   /// Store the Initial position of Gift as 1 size up than block
        }
        else {
            bullet.SetPosition(m_Position.X, m_Position.Y - (m_Height >> 1));   /// Store the Initial position of Gift as 1 size up than block
        }
        
        m_lBullet.push_back(bullet);
        
        m_PrevState = m_State;
        m_State = SHOOTING;
        LogDebug(BIT_PLAYER, "Player::Fire(), Number of Bullets : %d\n", m_lBullet.size());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Draw Bullets
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::DrawBullets(sf::RenderWindow &window) {
    auto it = m_lBullet.begin();
    for (; it != m_lBullet.end(); it++) {
        it->LoadBulletImage(window);
        if (Entity::DYING == it->GetState()) {
            m_lBullet.erase(it);
            LogInfo(BIT_MARIO, "Mario::DrawBullets(), Num Bullets %d \n", m_lBullet.size());
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Breif : Reset the plaer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Reset() {
    m_State = AIR;
    m_Position.X = PLAYER_START_X;
    m_Position.Y = PLAYER_START_Y;
    m_Size = SMALL;
    m_TileVector = {PLAYER_WIDTH, PLAYER_HEIGHT_SMALL};
    m_Speed = DEFAULT_SPEED;
    m_Ability = P1_REGULAR;
    m_JumpFactor = 0;
    m_bLifeReduced = false;
    m_bDying = false;
    m_ImgIdx = PlayerImgIdx::STAND;
}
