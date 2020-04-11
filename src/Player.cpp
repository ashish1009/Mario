#include "Player.h"
#include "Logger.h"

const short PLAYER_START_X = 40;
const short FRAME_MOVE_START_PLAYER_X = 80;     /// Player Position after which background will move left
const short PLAYER_POS_X_STOP = 112;            /// Player Position after witch only Background will move
const short MAX_FRAME_MOVE = 3130;              /// Frame can max move to

Player *Player::m_pInstance = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Constructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::Player()
: m_TileVector(PLAYER_WIDTH, PLAYER_HEIGHT_SMALL), m_PlayerImgIdx(PlayerImgIdx::STAND),
    m_PlayerHeight(PLAYER_HEIGHT_SMALL), m_PlayerMoveIdx(0), m_JumpFactor(0) {
    SetPosition(PLAYER_START_X, 150);
//        IncreaseSize();

    m_pObstacle = Obstacle::GetInstance();
    LogInfo(BIT_PLAYER, "Player::Player(), Player Constructor called !! \n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Destructor of Player Class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player::~Player() {
    m_pObstacle->ReleaseInstance();
    LogInfo(BIT_PLAYER, "Player::~Player(), Player Destructor called !! \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Create new Instance for Player. If alredy created then return the older Pointer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player *Player::GetInstance() {
    if (nullptr == m_pInstance) {
        LogInfo(BIT_PLAYER, "Player::GetInstance(), Creating Player Instance() \n");
        m_pInstance = new Player();
    }
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : delete the created Instance
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Player *Player::ReleaseInstance() {
    if (nullptr != m_pInstance) {
        LogInfo(BIT_PLAYER, "Player::ReleaseInstance(), Deleting Player Instance() \n");
        delete m_pInstance;
        m_pInstance = nullptr;
    }
    return m_pInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Load Player Image using Tile Map Classs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Player::LoadPlayerImage(sf::RenderWindow &winMario) {
    TileMap::PrintControl_s printControl;
    
    if (((GROUND == m_State) && (RUNNING != m_PrevState)) || (PlayerImgIdx::MID_INC == m_PlayerImgIdx)) {
        m_PlayerImgIdx = PlayerImgIdx::STAND;
    }
    
    ///  Get the Y Image Index for Player (each index contain both Small and Big Player)
    const int ImgPosY = m_Ability * (PLAYER_HEIGHT_BIG + PLAYER_HEIGHT_SMALL);
    
    printControl.tileSet = ResourcePath + PLAYER_IMG_PATH;
    printControl.position = m_Position;
    printControl.imgY = (SMALL == m_Size) ? ImgPosY + PLAYER_HEIGHT_BIG : ImgPosY;
    printControl.tile = m_PlayerImgIdx;
    printControl.tileSize = m_TileVector;
    printControl.bInverted = (LEFT == m_Direction) ? true : false;
    
    if (!m_Map.Load(printControl)) {
        LogError (BIT_PLAYER, "Player::LoadPlayerImage() : Can not Load Image using TileMap \n");
        return EXIT_FAILURE;
    }

    m_PlayerView.reset(sf::FloatRect(0.f, 0.f, WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT));
    winMario.setView(m_PlayerView);
    winMario.draw(m_Map);
    return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Check state of Playe(     if AIR then Free fall if jumping then jump
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::CheckPlayerState(const int frameX, sf::RenderWindow &winMario) {
    if (JUMPING == m_State) {
        Jump(frameX);
    }
    else {
        if (IsDownCollision(frameX)) {
            /// Land Player
            ResetSpeed();
            SetState(GROUND);                   /// if Landed change state to Ground
            ResetJumpFactor();
        }
        else {
            SetPosition(m_Position.X, m_Position.Y + 1);
        }
    }
    if (EXIT_FAILURE == LoadPlayerImage(winMario)) {
        LogError(BIT_MARIO, " Player::CheckPlayerStatus() : Can Not Load Player Image \n");
        winMario.close();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Increase size of player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::IncreaseSize() {
    SetSize(BIG);
    m_PlayerHeight = PLAYER_HEIGHT_BIG;
    m_TileVector = {PLAYER_WIDTH, PLAYER_HEIGHT_BIG};
    m_PlayerImgIdx = PlayerImgIdx::MID_INC;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      :  Jump Player
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Jump(const int frameX) {
    SetImgIdx(PlayerImgIdx::JUMP);
    
    if (MAX_JUMP_HEIGHT < m_JumpFactor) {
        SetState(AIR);                                      /// On reaching heighest lvel change state to Air so tha it may free fall
    }
    else {
        if (!IsJumpCollision(frameX)) {
            m_JumpFactor++;
            SetPosition(m_Position.X, m_Position.Y - 1);
        }
        else {
            SetState(AIR);                                  /// On collision change state to Air so tha it may free fall
        }
    }
    SetPrevState(JUMPING);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief      : Move the Player also changes the m_FramePositionX of Mario clsss
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Move(Entity::Direction_e direction, int &frameX, sf::RenderWindow &winMario) {
    SetDirection(direction);
    Obstacle *pObstacle = Obstacle::GetInstance();

    const int pixelToColloidU = m_PlayerHeight - gPixelToColloidD;                         /// First pixel from last to be collided : can be changed
    const int xPixelOfPlayer = (RIGHT == m_Direction) ? (m_Position.X + PLAYER_WIDTH + 1) : (m_Position.X - 1);

    if (GROUND == m_State) {
        SetState(RUNNING);
    }
    if (MAX_SPEED > m_Speed) {
        m_Speed ++;
    }
    
    for (int i = 0; i < m_Speed; i++) {
        if (RUNNING == m_State) {
            SetImgIdx(PlayerImgIdx::RUN[m_PlayerMoveIdx++]);
            m_PlayerMoveIdx %= PlayerImgIdx::RUN_IDX_ARR_SIZE;
        }
        else if (JUMPING == m_State) {
            Jump(frameX);
        }
        
        if (!IsSideCollision(frameX, pixelToColloidU, xPixelOfPlayer)) {
            if (m_Direction == RIGHT) {
                if (PLAYER_POS_X_STOP > m_Position.X) {
                    SetPosition(m_Position.X + 1, m_Position.Y);
                }
                else {
                    if (MAX_FRAME_MOVE > frameX) {
                        frameX++;
                    }
                }
            } /// if (m_Direction == RIGHT)
            else {
                if (0 <= m_Position.X) {
                    SetPosition(m_Position.X - 1, m_Position.Y);
                }
            }
            if (EXIT_FAILURE == LoadPlayerImage(winMario)) {
                LogError(BIT_MARIO, " Player::CheckPlayerStatus() : Can Not Load Player Image \n");
                winMario.close();
            }
        } /// (!IsSideCollision(frameX, pixelToColloidU, xPixelOfPlayer))
    } /// for (int i = 0; i < m_Speed; i++)
    SetPrevState(RUNNING);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : IF player is firable then fires the bullet
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Player::Fire() {
    if (FIRABLE == m_Ability) {
        std::cout << "Fire \n";
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the landing collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Player::IsDownCollision (const int frameX) {
    bool bIsLeftDownCollision = m_pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedL + m_Position.X + frameX);
    bool bIsRightDownCollision = m_pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedR + m_Position.X + frameX);
                            
    if (bIsLeftDownCollision || bIsRightDownCollision) {
        return true;
    }
    
    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedL - 2 + m_Position.X + frameX)) && (!bIsRightDownCollision)) {
        SetPosition (m_Position.X + gPixelToBeLandedL, m_Position.Y);
        SetState(AIR);
        return false;
    }
    
    /// Shift the plyer if few pixels are left for landing and those pixels are not enough for landing
    else if ((m_pObstacle->GetIsObstacleAt (m_Position.Y + 1, gPixelToBeLandedR + 2 + m_Position.X + frameX)) && (!bIsLeftDownCollision)) {
        SetPosition (m_Position.X - gPixelToBeLandedL, m_Position.Y);
        SetState(AIR);
        return false;
    }
    else {
        return  false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the Jumping collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Player::IsJumpCollision (const int frameX) {
    bool bIsLeftUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - m_PlayerHeight - 1, gPixelToBeUpColloidedL + m_Position.X + frameX);
    bool bIsRightUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - m_PlayerHeight - 1, gPixelToBeUpColloidedR + m_Position.X + frameX);
    
    /// Set the block as popped so that it may behave according to blocs's behaviour e.g Coin, Mushroom or none
    if (bIsLeftUpCollision || bIsRightUpCollision) {
        m_pObstacle->SetPoppedBlock (m_Position.Y - m_PlayerHeight - 1, m_Position.X + frameX + (PLAYER_WIDTH >> 1));
        return true;
    }
    
    /// Shift the plyer to Right if few pixels are left for up Collision and those pixels are not enough for Up Left Collision
    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - m_PlayerHeight - 1, gPixelToBeUpColloidedL + m_Position.X - 2 + frameX) && (!bIsRightUpCollision)) {
        SetPosition (m_Position.X + gPixelToBeUpColloidedL, m_Position.Y);
        return false;
    }
    
    /// Shift the plyer to Left if few pixels are left for up Collision and those pixels are not enough for Up right Collision
    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - m_PlayerHeight - 1, gPixelToBeUpColloidedR + m_Position.X + 2 + frameX) && (!bIsLeftUpCollision)) {
        SetPosition (m_Position.X - gPixelToBeUpColloidedL, m_Position.Y);
        return false;
    }
    
    else {
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Brief : Check the Side collison
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Player::IsSideCollision (const int frameX, const int pixelToColloidU, const int xPixelOfPlayer) {
    const int midPixel = (pixelToColloidU + gPixelToColloidD) >> 1;
    
    bool bIsSideUpCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - pixelToColloidU, xPixelOfPlayer + frameX);
    bool bIsSideDownCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - gPixelToColloidD, xPixelOfPlayer + frameX);
    bool bIsSideMidCollision = m_pObstacle->GetIsObstacleAt(m_Position.Y - midPixel, xPixelOfPlayer + frameX);
    
    if (bIsSideUpCollision || bIsSideDownCollision || bIsSideMidCollision) {
        return true;
    }
    
    /// Shift the plyer Up if few pixels are Down for landing and those pixels are not enough for for side collision
    else if (m_pObstacle->GetIsObstacleAt (m_Position.Y - gPixelToColloidD + 2, xPixelOfPlayer + frameX) && (!bIsSideDownCollision)) {
        SetPosition(m_Position.X + gPixelToBeLandedL, m_Position.Y - gPixelToColloidD - 3);
        return false;
    }
    
    else {
        return false;
    }
}

bool Player::IsPlayerCollision() {
    return false;
}
